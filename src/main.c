#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

#include "../include/util.h"
#include "../include/dns_client.h"
#include "../include/dns_server.h"
#include "../include/query_pool.h"

uv_loop_t * loop;
Rbtree * tree;
Query_Pool * qpool;
FILE * log_file;

int main(int argc, char * argv[])
{
    // TODO: 加注释
    init_config(argc, argv);
    log_file = stderr;
    if (LOG_PATH)
    {
        log_debug("打开log文件");
        log_file = fopen(LOG_PATH, "w");
        if (!log_file)
        {
            log_fatal("log文件打开失败");
            exit(1);
        }
    }
    
    FILE * hosts_file = fopen(HOSTS_PATH, "r");
    if (!hosts_file)
    {
        log_fatal("hosts文件打开失败");
        exit(1);
    }
    
    log_info("启动DNS中继服务器")
    loop = uv_default_loop();
    tree = cache_init(hosts_file);
    qpool = qpool_init(loop, tree);
    init_client(loop);
    init_server(loop);
    return uv_run(loop, UV_RUN_DEFAULT);
}
