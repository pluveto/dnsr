/**
 * @file      dns_print.c
 * @brief     DNS报文打印
 * @author    Ziheng Mao
 * @date      2021/4/6
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件的内容是打印DNS报文的实现。
*/

#include "../include/dns_print.h"

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include "../include/util.h"

void print_dns_string(const char * pstring, unsigned int len)
{
    if (!(LOG_MASK & 1))return;
    log_debug("DNS报文字节流：");
    for (unsigned int i = 0; i < len; i++)
    {
        if (i % 16 == 0)
        {
            if (i)fprintf(log_file, "\n");
            fprintf(log_file, "%04x ", i);
        }
        fprintf(log_file, "%02hhx ", pstring[i]);
    }
    fprintf(log_file, "\n");
}

/**
 * @brief 打印A类型RR的rdata字段
 *
 * @param rdata rdata字段
 */
static void print_rr_A(const uint8_t * rdata)
{
    fprintf(log_file, "%d.%d.%d.%d", rdata[0], rdata[1], rdata[2], rdata[3]);
}

/**
 * @brief 打印AAAA类型RR的rdata字段
 *
 * @param rdata rdata字段
 */
static void print_rr_AAAA(const uint8_t * rdata)
{
    for (int i = 0; i < 16; i += 2)
    {
        if (i)fprintf(log_file, ":");
        fprintf(log_file, "%x", (rdata[i] << 8) + rdata[i + 1]);
    }
}

/**
 * @brief 打印CNAME类型RR的rdata字段
 *
 * @param rdata rdata字段
 */
static void print_rr_CNAME(const uint8_t * rdata)
{
    fprintf(log_file, "%s", rdata);
}

/**
 * @brief 打印SOA类型RR的rdata字段
 *
 * @param rdlength rdlength字段
 * @param rdata rdata字段
 */
static void print_rr_SOA(uint16_t rdlength, const uint8_t * rdata)
{
    print_rr_CNAME(rdata);
    fprintf(log_file, " ");
    print_rr_CNAME(rdata + strlen(rdata) + 1);
    fprintf(log_file, " ");
    fprintf(log_file, "%" PRIu32 " ", ntohl(*(uint32_t *) (rdata + rdlength - 20)));
    fprintf(log_file, "%" PRIu32 " ", ntohl(*(uint32_t *) (rdata + rdlength - 16)));
    fprintf(log_file, "%" PRIu32 " ", ntohl(*(uint32_t *) (rdata + rdlength - 12)));
    fprintf(log_file, "%" PRIu32 " ", ntohl(*(uint32_t *) (rdata + rdlength - 8)));
    fprintf(log_file, "%" PRIu32, ntohl(*(uint32_t *) (rdata + rdlength - 4)));
}

/**
 * @brief 打印Header Section
 *
 * @param phead Header Section
 */
static void print_dns_header(const Dns_Header * phead)
{
    fprintf(log_file, "ID = 0x%04" PRIx16 "\n", phead->id);
    fprintf(log_file, "QR = %" PRIu8 "\n", phead->qr);
    fprintf(log_file, "OPCODE = %" PRIu8 "\n", phead->opcode);
    fprintf(log_file, "AA = %" PRIu8 "\n", phead->aa);
    fprintf(log_file, "TC = %" PRIu8 "\n", phead->tc);
    fprintf(log_file, "RD = %" PRIu8 "\n", phead->rd);
    fprintf(log_file, "RA = %" PRIu8 "\n", phead->ra);
    fprintf(log_file, "RCODE = %" PRIu16 "\n", phead->rcode);
    fprintf(log_file, "QDCOUNT = %" PRIu16 "\n", phead->qdcount);
    fprintf(log_file, "ANCOUNT = %" PRIu16 "\n", phead->ancount);
    fprintf(log_file, "NSCOUNT = %" PRIu16 "\n", phead->nscount);
    fprintf(log_file, "ARCOUNT = %" PRIu16 "\n", phead->arcount);
}

/**
 * @brief 打印Question Section
 *
 * @param pque Question Section
 */
static void print_dns_question(const Dns_Que * pque)
{
    fprintf(log_file, "QNAME = %s\n", pque->qname);
    fprintf(log_file, "QTYPE = %" PRIu16 "\n", pque->qtype);
    fprintf(log_file, "QCLASS = %" PRIu16 "\n", pque->qclass);
}

/**
 * @brief 打印Resource Record
 *
 * @param prr Resource Record
 */
static void print_dns_rr(const Dns_RR * prr)
{
    fprintf(log_file, "NAME = %s\n", prr->name);
    fprintf(log_file, "TYPE = %" PRIu16 "\n", prr->type);
    fprintf(log_file, "CLASS = %" PRIu16 "\n", prr->class);
    fprintf(log_file, "TTL = %" PRIu32 "\n", prr->ttl);
    fprintf(log_file, "RDLENGTH = %" PRIu16 "\n", prr->rdlength);
    fprintf(log_file, "RDATA = ");
    if (prr->type == DNS_TYPE_A)
        print_rr_A(prr->rdata);
    else if (prr->type == DNS_TYPE_CNAME || prr->type == DNS_TYPE_NS)
        print_rr_CNAME(prr->rdata);
    else if (prr->type == DNS_TYPE_AAAA)
        print_rr_AAAA(prr->rdata);
    else if (prr->type == DNS_TYPE_SOA)
        print_rr_SOA(prr->rdlength, prr->rdata);
    else
        for (int i = 0; i < prr->rdlength; ++i)
            fprintf(log_file, "%" PRIu8, *(prr->rdata + i));
    fprintf(log_file, "\n");
}

void print_dns_message(const Dns_Msg * pmsg)
{
    if (!(LOG_MASK & 1))return;
    log_debug("DNS报文内容：");
    fprintf(log_file, "=======Header==========\n");
    print_dns_header(pmsg->header);
    fprintf(log_file, "\n");
    fprintf(log_file, "=======Question========\n");
    for (Dns_Que * pque = pmsg->que; pque; pque = pque->next)
    {
        print_dns_question(pque);
        fprintf(log_file, "\n");
    }
    Dns_RR * prr = pmsg->rr;
    fprintf(log_file, "=======Answer==========\n");
    for (int i = 0; i < pmsg->header->ancount; ++i, prr = prr->next)
    {
        print_dns_rr(prr);
        fprintf(log_file, "\n");
    }
    fprintf(log_file, "=======Authority=======\n");
    for (int i = 0; i < pmsg->header->nscount; ++i, prr = prr->next)
    {
        print_dns_rr(prr);
        fprintf(log_file, "\n");
    }
    fprintf(log_file, "=======Additional======\n");
    for (int i = 0; i < pmsg->header->arcount; ++i, prr = prr->next)
    {
        print_dns_rr(prr);
        fprintf(log_file, "\n");
    }
}
