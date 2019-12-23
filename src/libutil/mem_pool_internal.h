/*-
 * Copyright 2019 Vsevolod Stakhov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef RSPAMD_MEM_POOL_INTERNAL_H
#define RSPAMD_MEM_POOL_INTERNAL_H

/*
 * Internal memory pool stuff
 */

#define align_ptr(p, a)                                                   \
    (guint8 *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))

enum rspamd_mempool_chain_type {
	RSPAMD_MEMPOOL_NORMAL = 0,
	RSPAMD_MEMPOOL_SHARED,
	RSPAMD_MEMPOOL_MAX
};
#define ENTRY_LEN 128
#define ENTRY_NELTS 64

struct entry_elt {
	guint32 fragmentation;
	guint32 leftover;
};

struct rspamd_mempool_entry_point {
	gchar src[ENTRY_LEN];
	guint32 cur_suggestion;
	guint32 cur_elts;
	struct entry_elt elts[ENTRY_NELTS];
};

/**
 * Destructors list item structure
 */
struct _pool_destructors {
	rspamd_mempool_destruct_t func;         /**< pointer to destructor					*/
	void *data;                             /**< data to free							*/
	const gchar *function;                  /**< function from which this destructor was added */
	const gchar *loc;                       /**< line number                            */
	struct _pool_destructors *next;
};

struct rspamd_mempool_specific {
	struct _pool_chain *pools[RSPAMD_MEMPOOL_MAX];
	struct _pool_destructors *dtors_head, *dtors_tail;
	GPtrArray *trash_stack;
	GHashTable *variables;                  /**< private memory pool variables			*/
	gsize elt_len;                            /**< size of an element						*/
	gsize used_memory;
	gsize wasted_memory;
	struct rspamd_mempool_entry_point *entry;
};

/**
 * Pool page structure
 */
struct _pool_chain {
	guint8 *begin;                  /**< begin of pool chain block              */
	guint8 *pos;                    /**< current start of free space in block   */
	gsize slice_size;                      /**< length of block                        */
	rspamd_mempool_mutex_t *lock;
	struct _pool_chain *next;
};


#endif
