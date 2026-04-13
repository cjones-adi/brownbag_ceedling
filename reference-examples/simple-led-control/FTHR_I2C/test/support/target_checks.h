/**
 * @file    target_checks.h
 * @brief   Target-specific check macros for unit tests.
 *          Included in the Ceedling :support path so tests can
 *          optionally use TARGET_NUM / TARGET defines in assertions.
 */

#ifndef TARGET_CHECKS_H
#define TARGET_CHECKS_H

/* These are injected as compiler defines via project.yml :defines,
 * but the header is provided here for editors / static-analysis tools. */
#ifndef TARGET_NUM
#define TARGET_NUM 32655
#endif

#ifndef TARGET
#define TARGET MAX32655
#endif

#endif /* TARGET_CHECKS_H */
