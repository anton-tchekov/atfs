#ifndef __PATH_H__
#define __PATH_H__

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
char *path_join(char *path, const char *append);

/**
 * @brief Remove the last component of the path in place.
 *        The validity of the path is not checked.
 *
 * @param path The path to remove the last part of
 * @return Parent path
 */
char *path_parent(char *path);

/**
 * @brief Get a pointer to the path excluding the first component
 *
 * @param path The path to remove the first part of
 * @return Rest of the path
 */
const char *path_rest(const char *path);

/**
 * @brief Check if a path is valid.
 *
 * @param path The path to check
 * @return true if valid, else false
 */
bool path_valid(const char *path);

/**
 * @brief Check if a identifier is valid.
 *
 * @param ident The identifier to check
 * @return true if valid, else false
 */
bool identifier_valid(const char *path);

#endif /* __PATH_H__ */