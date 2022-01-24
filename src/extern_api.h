/**
 * @file extern_api.h
 * @author Ben Menking <ben@menking.net>
 * @brief Utilizes a named pipe for webhook and API feature
 * @version 0.1
 * @date 2022-01-24
 * 
 * Copyright (c) 2022 Ben Menking.
 */

/**
 * @brief Set the up pipe object
 * 
 */
void setup_pipes();

/**
 * @brief post a message to the webhook pipe
 * 
 * @return int 
 */
int postMessage(char *);

/**
 * @brief attempt to read from the api pipe
 * 
 */
int getMessage(char *, int);

/**
 * @brief 
 * 
 */
void teardown_pipes();