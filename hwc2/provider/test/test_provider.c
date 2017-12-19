//
// Created by Alessandro Iori on 10/12/2017.
//
#include "test_provider.h"

/* Support */
/*
buffer_concurrent_t* TMP_C_BUFFER;
pthread_t TMP_DISPATCHER;
int TMP_MSGS_NUMBER;
*/

int PROVIDER_FAKE_DISPATCHER_MSG_CNT;
msg_t* PROVIDER_FAKE_DISPATCHER_LAST_MSG;
/*
void test_support_provider_cond_wait_while_init(void)
{
    EXIT_FROM_COND_WAIT_WHILE = (int*) malloc(sizeof(int));
    *EXIT_FROM_COND_WAIT_WHILE = 1;
}

void test_support_provider_cond_wait_while_destroy(void)
{
    free(EXIT_FROM_COND_WAIT_WHILE);
}
 */

void test_support_provider_clean_fake_dispatcher(void)
{
    PROVIDER_FAKE_DISPATCHER_MSG_CNT = 0;
    if(PROVIDER_FAKE_DISPATCHER_LAST_MSG != (msg_t*)NULL)
    {
        PROVIDER_FAKE_DISPATCHER_LAST_MSG->msg_destroy(PROVIDER_FAKE_DISPATCHER_LAST_MSG);
    }
}


void* test_support_provider_fake_dispatcher_function(void* args)
{
    buffer_concurrent_t* c_buffer = (buffer_concurrent_t*) args;

    PROVIDER_FAKE_DISPATCHER_MSG_CNT = 0;

    msg_t* m;
    int exit = 0;
    while(!exit)
    {
        PROVIDER_FAKE_DISPATCHER_LAST_MSG = (msg_t*) NULL;
        PROVIDER_FAKE_DISPATCHER_LAST_MSG = buffer_concurrent_get_msg(c_buffer);

        if(0 == strcmp(PROVIDER_FAKE_DISPATCHER_LAST_MSG->content, POISON_PILL->content))
        {
            exit = 1;
        }
        else
        {
            PROVIDER_FAKE_DISPATCHER_MSG_CNT++;
            //dispatcher_send_msg_to_all_reader(DISPATCHER_READERS_LIST, DISPATCHER_LAST_MSG);
            //printf("\r\n Invio a tutti reader: %s", DISPATCHER_LAST_MSG->content);
        }
    }
    //dispatcher_send_msg_to_all_reader(DISPATCHER_READERS_LIST, POISON_PILL);
    //printf("\r\n Invio a tutti reader: %s", POISON_PILL->content);

    return (void*) NULL;

    /*
    while(TMP_MSGS_NUMBER > 0)
    {
        TMP_MSGS_NUMBER--;
        buffer_concurrent_get_msg(TMP_C_BUFFER);
    }
    TMP_C_BUFFER = (buffer_concurrent_t*) NULL;
    return (void*) NULL;
    */
}

void test_support_provider_fake_dispatcher(buffer_concurrent_t* c_buffer)
{
    pthread_t      tid;  // thread ID
    pthread_attr_t attr; // thread attribute

    // set thread detachstate attribute to DETACHED
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    if(pthread_create(&tid, &attr, test_support_provider_fake_dispatcher_function, c_buffer))
    {
        printf("error creating dispatcher thread\t\n");
        exit(1);
    }
    /*
    TMP_C_BUFFER = c_buffer;
    TMP_MSGS_NUMBER = msg_number;

    if(pthread_create(&TMP_DISPATCHER, NULL, test_support_provider_fake_dispatcher_function, NULL))
    {
        printf("error creating dispatcher thread\t\n");
        exit(1);
    }
     */
}

/*
void test_support_provider_join_fake_dispatcher(void)
{
    if(pthread_join(TMP_DISPATCHER, NULL))
    {
        printf("error joining dispatcher thread\t\n");
        exit(1);
    }
}
 */

/* Test */
int provider_before(void)
{
    return 0;
}

int provider_after(void)
{
    return 0;
}

void test_provider_init(void) {
    int msg_len = 1;
    msg_t *msg = msg_init_string("content");
    buffer_concurrent_t *buffer = buffer_concurrent_init(1);

    provider_t *provider = provider_init(buffer, msg, &msg_len);

    CU_ASSERT_PTR_NOT_NULL(provider);
    CU_ASSERT_PTR_NOT_NULL(PROVIDER_DISPATCHER_BUFFER);
    CU_ASSERT_PTR_NOT_NULL(PROVIDER_MSGS);
    CU_ASSERT(1 == *PROVIDER_MSGS_LEN);

    msg->msg_destroy(msg);
    buffer->buffer_concurrent_destroy(buffer);
    free(provider);
}

void test_provider_destroy(void)
{
    int msg_len = 1;
    msg_t *msg = msg_init_string("content");
    buffer_concurrent_t *buffer = buffer_concurrent_init(1);
    provider_t* provider = provider_init(buffer, msg, &msg_len);

    buffer->buffer_concurrent_destroy(buffer);
    provider->provider_destroy(provider);
    // basta che il test termini correttamente: no asserzioni!
}

void test_provider_1_msg_spediti_buffer_dim_2(void)
{
    char content[] = "content";
    int msg_len = 1;
    msg_t *msg = msg_init_string(content);
    buffer_concurrent_t* c_buffer = buffer_concurrent_init(1+1); //msg + poison_pill
    provider_t *provider = provider_init(c_buffer, msg, &msg_len);

    provider_start_thread(provider);
    sleep(3);

    CU_ASSERT(2 == *c_buffer->buffer->p_size);
    CU_ASSERT(0 == strcmp(c_buffer->buffer->msgs[0].content, content));
    CU_ASSERT(0 == strcmp(c_buffer->buffer->msgs[1].content, POISON_PILL->content));

    provider->provider_destroy(provider);
    c_buffer->buffer_concurrent_destroy(c_buffer);
    msg->msg_destroy(msg);
}

void test_provider_2_msg_spediti_buffer_dim_4(void)
{
    char content[] = "content";
    int msg_len = 2;
    msg_t* msg = msg_init_string(content);
    msg_t msgs[] = {*msg, *msg};
    buffer_concurrent_t* c_buffer = buffer_concurrent_init(4);
    provider_t *provider = provider_init(c_buffer, msgs, &msg_len);

    provider_start_thread(provider);
    sleep(5);

    CU_ASSERT(2+1 == *c_buffer->buffer->p_size); //msg + poison_pill
    CU_ASSERT(0 == strcmp(c_buffer->buffer->msgs[0].content, content));
    CU_ASSERT(0 == strcmp(c_buffer->buffer->msgs[1].content, content));
    CU_ASSERT(0 == strcmp(c_buffer->buffer->msgs[2].content, POISON_PILL->content));
    CU_ASSERT_PTR_NULL(c_buffer->buffer->msgs[3].content);

    provider->provider_destroy(provider);
    c_buffer->buffer_concurrent_destroy(c_buffer);
    msg->msg_destroy(msg);
}

void test_provider_2_msg_spediti_buffer_dim_1(void)
{
    char content[] = "content";
    int msg_len = 2;
    msg_t* msg = msg_init_string(content);
    msg_t msgs[] = {*msg, *msg};
    buffer_concurrent_t* c_buffer = buffer_concurrent_init(1);
    provider_t *provider = provider_init(c_buffer, msgs, &msg_len);

    provider_start_thread(provider);
    sleep(2);
    test_support_provider_fake_dispatcher(c_buffer);
    sleep(4);

    CU_ASSERT(0 == *c_buffer->buffer->p_size);
    CU_ASSERT(2 == PROVIDER_FAKE_DISPATCHER_MSG_CNT);
    CU_ASSERT(0 == strcmp(PROVIDER_FAKE_DISPATCHER_LAST_MSG->content, POISON_PILL->content));

    test_support_provider_clean_fake_dispatcher();
    provider->provider_destroy(provider);
    c_buffer->buffer_concurrent_destroy(c_buffer);
    msg->msg_destroy(msg);
}

void test_provider_10_msg_spediti_buffer_dim_5(void)
{
    char content[] = "content";
    int msg_len = 10;
    msg_t* msg = msg_init_string(content);
    msg_t msgs[] = {*msg, *msg, *msg, *msg, *msg, *msg, *msg, *msg, *msg, *msg};
    buffer_concurrent_t* c_buffer = buffer_concurrent_init(5);
    provider_t *provider = provider_init(c_buffer, msgs, &msg_len);

    provider_start_thread(provider);
    sleep(2);
    test_support_provider_fake_dispatcher(c_buffer);
    sleep(5);

    CU_ASSERT(0 == *c_buffer->buffer->p_size);
    CU_ASSERT(10 == PROVIDER_FAKE_DISPATCHER_MSG_CNT);
    CU_ASSERT(0 == strcmp(PROVIDER_FAKE_DISPATCHER_LAST_MSG->content, POISON_PILL->content));

    test_support_provider_clean_fake_dispatcher();
    provider->provider_destroy(provider);
    c_buffer->buffer_concurrent_destroy(c_buffer);
    msg->msg_destroy(msg);
}
