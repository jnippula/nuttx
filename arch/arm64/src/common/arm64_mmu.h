/****************************************************************************
 * arch/arm64/src/common/arm64_mmu.h
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 *
 ****************************************************************************/

#ifndef __ARCH_ARM64_SRC_COMMON_ARM64_MMU_H
#define __ARCH_ARM64_SRC_COMMON_ARM64_MMU_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "arm64_arch.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Following Memory types supported through MAIR encodings can be passed
 * by user through "attrs"(attributes) field of specified memory region.
 * As MAIR supports such 8 encodings, we will reserve attrs[2:0];
 * so that we can provide encodings upto 7 if needed in future.
 *
 * See Arm® Architecture Reference Manual, ARM DDI 0487E.a, B2.7.2
 */
#define MT_TYPE_MASK                0x7U
#define MT_TYPE(attr)               ((attr) & MT_TYPE_MASK)
#define MT_DEVICE_NGNRNE            0U
#define MT_DEVICE_NGNRE             1U
#define MT_DEVICE_GRE               2U
#define MT_NORMAL_NC                3U
#define MT_NORMAL                   4U

#define MEMORY_ATTRIBUTES           ((0x00 << (MT_DEVICE_NGNRNE * 8)) |  \
                                     (0x04 << (MT_DEVICE_NGNRE * 8))   | \
                                     (0x0c << (MT_DEVICE_GRE * 8))     | \
                                     (0x44 << (MT_NORMAL_NC * 8))      | \
                                     (0xffUL << (MT_NORMAL * 8)))

/* More flags from user's perpective are supported using remaining bits
 * of "attrs" field, i.e. attrs[31:3], underlying code will take care
 * of setting PTE fields correctly.
 *
 * current usage of attrs[31:3] is:
 * attrs[3] : Access Permissions
 * attrs[4] : Memory access from secure/ns state
 * attrs[5] : Execute Permissions
 *
 * See Arm® Architecture Reference Manual, ARM DDI 0487E.a
 */

#define MT_PERM_SHIFT               3U
#define MT_SEC_SHIFT                4U
#define MT_EXECUTE_SHIFT            5U

#define MT_RO                       (0U << MT_PERM_SHIFT)
#define MT_RW                       (1U << MT_PERM_SHIFT)

#define MT_SECURE                   (0U << MT_SEC_SHIFT)
#define MT_NS                       (1U << MT_SEC_SHIFT)

#define MT_EXECUTE                  (0U << MT_EXECUTE_SHIFT)
#define MT_EXECUTE_NEVER            (1U << MT_EXECUTE_SHIFT)

/* Some compound attributes for most common usages */

#define MT_CODE                     (MT_NORMAL | MT_RO | MT_EXECUTE)
#define MT_RODATA                   (MT_NORMAL | MT_RO | MT_EXECUTE_NEVER)

/* PTE descriptor can be Block descriptor or Table descriptor
 * or Page descriptor.
 */

#define PTE_DESC_TYPE_MASK          3U
#define PTE_BLOCK_DESC              1U
#define PTE_TABLE_DESC              3U
#define PTE_PAGE_DESC               3U
#define PTE_INVALID_DESC            0U

/* Block and Page descriptor attributes fields */

#define PTE_BLOCK_DESC_MEMTYPE(x)   ((x) << 2)
#define PTE_BLOCK_DESC_NS           (1ULL << 5)
#define PTE_BLOCK_DESC_AP_RO        (1ULL << 7)
#define PTE_BLOCK_DESC_AP_RW        (0ULL << 7)
#define PTE_BLOCK_DESC_NON_SHARE    (0ULL << 8)
#define PTE_BLOCK_DESC_OUTER_SHARE  (2ULL << 8)
#define PTE_BLOCK_DESC_INNER_SHARE  (3ULL << 8)
#define PTE_BLOCK_DESC_AF           (1ULL << 10)
#define PTE_BLOCK_DESC_NG           (1ULL << 11)
#define PTE_BLOCK_DESC_PXN          (1ULL << 53)
#define PTE_BLOCK_DESC_UXN          (1ULL << 54)

/* TCR definitions.
 *
 * See Arm® Architecture Reference Manual, ARM DDI 0487E.a D13.2.112~114
 *
 */

#define TCR_EL1_IPS_SHIFT           32U
#define TCR_EL2_PS_SHIFT            16U
#define TCR_EL3_PS_SHIFT            16U

#define TCR_T0SZ_SHIFT              0U
#define TCR_T0SZ(x)  ((64 - (x)) << TCR_T0SZ_SHIFT)

#define TCR_IRGN_NC                 (0ULL << 8)
#define TCR_IRGN_WBWA               (1ULL << 8)
#define TCR_IRGN_WT                 (2ULL << 8)
#define TCR_IRGN_WBNWA              (3ULL << 8)
#define TCR_IRGN_MASK               (3ULL << 8)
#define TCR_ORGN_NC                 (0ULL << 10)
#define TCR_ORGN_WBWA               (1ULL << 10)
#define TCR_ORGN_WT                 (2ULL << 10)
#define TCR_ORGN_WBNWA              (3ULL << 10)
#define TCR_ORGN_MASK               (3ULL << 10)
#define TCR_SHARED_NON              (0ULL << 12)
#define TCR_SHARED_OUTER            (2ULL << 12)
#define TCR_SHARED_INNER            (3ULL << 12)
#define TCR_TG0_4K                  (0ULL << 14)
#define TCR_TG0_64K                 (1ULL << 14)
#define TCR_TG0_16K                 (2ULL << 14)
#define TCR_EPD1_DISABLE            (1ULL << 23)

#define TCR_PS_BITS_4GB             0x0ULL
#define TCR_PS_BITS_64GB            0x1ULL
#define TCR_PS_BITS_1TB             0x2ULL
#define TCR_PS_BITS_4TB             0x3ULL
#define TCR_PS_BITS_16TB            0x4ULL
#define TCR_PS_BITS_256TB           0x5ULL

#define CTR_EL0_DMINLINE_SHIFT      16
#define CTR_EL0_DMINLINE_MASK       BIT_MASK(4)
#define CTR_EL0_CWG_SHIFT           24
#define CTR_EL0_CWG_MASK            BIT_MASK(4)

/* clidr_el1 */

#define CLIDR_EL1_LOC_SHIFT          24
#define CLIDR_EL1_LOC_MASK           BIT_MASK(3)
#define CLIDR_EL1_CTYPE_SHIFT(level) ((level) * 3)
#define CLIDR_EL1_CTYPE_MASK         BIT_MASK(3)

/* ccsidr_el1 */

#define CCSIDR_EL1_LN_SZ_SHIFT       0
#define CCSIDR_EL1_LN_SZ_MASK        BIT_MASK(3)
#define CCSIDR_EL1_WAYS_SHIFT        3
#define CCSIDR_EL1_WAYS_MASK         BIT_MASK(10)
#define CCSIDR_EL1_SETS_SHIFT        13
#define CCSIDR_EL1_SETS_MASK         BIT_MASK(15)

/* Convenience macros to represent the ARMv8-A-specific
 * configuration for memory access permission and
 * cache-ability attribution.
 */

#define MMU_REGION_ENTRY(_name, _base_pa, _base_va, _size, _attrs) \
  {                                                                \
    .name       = (_name),                                         \
    .base_pa    = (_base_pa),                                      \
    .base_va    = (_base_va),                                      \
    .size       = (_size),                                         \
    .attrs      = (_attrs),                                        \
  }

#define MMU_REGION_FLAT_ENTRY(name, adr, sz, attrs) \
  MMU_REGION_ENTRY(name, adr, adr, sz, attrs)

#ifndef __ASSEMBLY__

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Region definition data structure */

struct arm_mmu_region
{
  /* Region Base Physical Address */

  uint64_t base_pa;

  /* Region Base Virtual Address */

  uint64_t base_va;

  /* Region size */

  uint64_t size;

  /* Region Name */

  const char *name;

  /* Region Attributes */

  unsigned int attrs;
};

/* MMU configuration data structure */

struct arm_mmu_config
{
  /* Number of regions */

  uint32_t num_regions;

  /* Regions */

  const struct arm_mmu_region *mmu_regions;
};

struct arm_mmu_ptables
{
  uint64_t *base_xlat_table;
};

/* Reference to the MMU configuration.
 *
 * This struct is defined and populated for each SoC,
 * and holds the build-time configuration information for the fixed MMU
 * regions enabled during kernel initialization.
 */

extern const struct arm_mmu_config g_mmu_config;
extern size_t g_dcache_line_size;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

int arm64_mmu_init(bool is_primary_core);

#endif /* __ASSEMBLY__ */

#endif /* __ARCH_ARM64_SRC_COMMON_ARM64_MMU_H */
