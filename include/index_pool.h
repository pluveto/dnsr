/**
 * @file      index_pool.h
 * @brief     序号池
 * @author    Ziheng Mao
 * @date      2021/4/5
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件中定义了一个序号池，用于分配发送的DNS报文的序号，并将序号和DNS查询对应，定义了判断池满、插入、查询、删除的接口。
 *
 * 当需要分配序号时，用户首先需要创建一个Index结构体，将结构体作为参数传入insert函数，该函数会返回分配的序号。
 *
 * 当序号使用完毕，用户调用delete函数，将该序号从序号池中移除，该函数返回序号对应的Index结构体。
*/

#ifndef DNSR_INDEX_POOL_H
#define DNSR_INDEX_POOL_H

#include <stdbool.h>

#include "queue.h"

#define INDEX_POOL_MAX_SIZE 65535

/// 序号结构体
typedef struct
{
    uint16_t id; ///< 发送的DNS查询报文的序号
    uint16_t prev_id; ///< 对应查询的序号
} Index;

/// 序号池
typedef struct index_pool
{
    Index * pool[INDEX_POOL_MAX_SIZE]; ///< 序号池
    unsigned short count; ///< 池中序号的数量
    Queue * queue; ///< 未分配的序号的队列
    
    /**
     * @brief 判断序号池是否已满
     *
     * @param ipool 序号池
     * @return 如果池已满，返回true
     */
    bool (* full)(struct index_pool * ipool);
    
    /**
     * @brief 分配一个新序号
     *
     * @param ipool 序号池
     * @param req 序号结构体
     * @return 分配的新序号
     */
    uint16_t (* insert)(struct index_pool * ipool, Index * req);
    
    /**
     * @brief 查询序号是否在序号池中
     *
     * @param ipool 序号池
     * @param index 序号
     * @return 如果在序号池中，返回true
     */
    bool (* query)(struct index_pool * ipool, uint16_t index);
    
    /**
     * @brief 从序号池中删除序号
     *
     * @param ipool 序号池
     * @param index 待删除的序号
     * @return 删除的序号对应的序号结构体
     */
    Index * (* delete)(struct index_pool * ipool, uint16_t index);
    
    /**
     * @brief 销毁序号池
     *
     * @param ipool 序号池
     */
    void (* destroy)(struct index_pool * ipool);
} Index_Pool;

/**
 * @brief 创建序号池
 *
 * @return 新的序号池
 */
Index_Pool * new_ipool();

#endif //DNSR_INDEX_POOL_H
