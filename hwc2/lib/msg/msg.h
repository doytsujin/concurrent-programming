//
// Created by Alessandro Iori on 19/11/2017.
//

#ifndef UNTITLED_MSG_H
#define UNTITLED_MSG_H

#include <stdlib.h>
#include <string.h>

#define MESSAGE_NULL  (msg_t *) NULL

typedef struct msg {
    void* content;                          // generico contenuto del messaggio
    void (*msg_destroy)(struct msg *);      // deallocazione msg
    struct msg* (*msg_copy)(struct msg *);  // creazione/copia msg
} msg_t;

msg_t* msg_init_string(void* content);

void msg_destroy_string( msg_t* msg);
msg_t* msg_copy_string( msg_t* msg);

#endif //UNTITLED_MSG_H
