/**
 * @file    atfs_path.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    18.09.2023
 * @brief   ATFS Path Utility Functions
 */

#ifndef __ATFS_PATH_H__
#define __ATFS_PATH_H__

#include "types.h"

/**
 * @brief Join two paths by copying the second path to the end of the first
 *        path and adding a dot in between. The first string must have enough
 *        space to contain the whole path.
 *        The validity of the path is not checked.
 *
 * @param path First part of the path
 * @param append Path that should be added to the end of the first path
 * @return Joined path
 */
char *atfs_path_join(char *path, const char *append);

/**
 * @brief Remove the last component of the path in place.
 *        The validity of the path is not checked.
 *
 * @param path The path to remove the last part of
 * @return Parent path
 */
char *atfs_path_parent(char *path);

/**
 * @brief Return the length of the first path component.
 *        The validity of the path is not checked.
 *
 * @param path The path
 * @return First component length
 */
size_t atfs_path_first_len(const char *path);

/**
 * @brief Get a pointer to the path excluding the first component
 *
 * @param path The path to remove the first part of
 * @return Rest of the path
 */
const char *atfs_path_rest(const char *path);

/**
 * @brief Check if a path is valid.
 *
 * @param path The path to check
 * @return 1 if valid, else 0
 */
int atfs_path_valid(const char *path);

#endif /* __ATFS_PATH_H__ */
