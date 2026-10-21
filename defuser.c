#include "defuser.h"

#include <inttypes.h>
#include <stdio.h>

#define MAX_FIELDS 8

/*
 * 把 values 中的 count 个整数写成一行“空格分隔的无符号十进制整数”，存入 out。
 * 成功返回 0；count 为 0（还没有实现）、snprintf 出错或结果被截断时返回非零。
 */
static int write_answer(char *out, size_t cap, const uint32_t *values, size_t count) {
    size_t used = 0;
    for (size_t i = 0; i < count; ++i) {
        int size = snprintf(out + used, cap - used, "%s%" PRIu32, i ? " " : "", values[i]);
        if (size < 0 || (size_t)size >= cap - used) return 1;
        used += (size_t)size;
    }
    return count == 0;
}

uint32_t p1_compute(uint64_t challenge){
    uint32_t low32 = (uint32_t)challenge;
    uint32_t high32 = (uint32_t)(challenge>>32);
    uint32_t mid32 = (uint32_t)(challenge>>16);

    uint32_t s0 = (3*low32) ^ (high32+0xbd99);
    uint32_t s1 = (mid32^0xb807) + 4*high32;
    uint32_t s2 = (s1-low32)*5 + 0xe5ec;
    return s2^s0;
}
int solve_phase_1(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_01 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    values[0] = p1_compute(challenge);
    count++;
    return write_answer(out, cap, values, count);
}

int solve_phase_2(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_02 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    values[0] = (uint32_t)(challenge>>1) & 0x7f;
    uint32_t shift = 0x1a;
    for(int i=1;i<6;i++){
        values[i] = (((challenge>>shift) & 0x1f) + i) ^ (3*values[i-1]);
        shift += 5;
    }
    count = 6;
    return write_answer(out, cap, values, count);
}

uint32_t p3_dispatch(uint32_t selector,uint32_t first){
    switch(selector){
        case 0: return ((first^0xe6b) + (first>>2));//p3_case_5
        case 1: return (5*first + 0xf8bc);//p3_case_4
        case 2: return ((4*first) ^ 0xe522);//p3_case_6
        case 3: return ((2*first) ^ 0x66e1);//p3_case_3
        case 4: return (5*first + 0x2a120);//p3_case_7
        case 5: return (5*first + 0x27655);//p3_case_1
        case 6: return ((first^0x3a3a) + (first>>1));//p3_case_2
        case 7: return (5*first + 0x94c3);//p3_case_0
        default: return 0;
    }
}
int solve_phase_3(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_03 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    values[0] = ((uint32_t)challenge) & 0x7u;
    uint32_t temp = (uint32_t)(challenge>>3) ^ (uint32_t)(challenge>>0x20);
    values[1] = p3_dispatch(values[0],temp);
    count = 2;
    return write_answer(out, cap, values, count);
}

uint32_t p4_walk(uint32_t target,uint32_t low,uint32_t up,uint32_t sth){
    uint32_t mid = ((up - low)>>1) + low;
    if(target == mid){
        return (mid+1) ^ sth;
    }else{
        uint32_t sth1 = mid + sth;
        if(target < mid){
            uint32_t temp = p4_walk(target,low,mid-1,sth);
            temp *= 5;
            return temp ^ sth1 ^ 0x14;
        }else{
            uint32_t temp = p4_walk(target,mid+1,up,sth);
            temp *= 5;
            return temp ^ sth1 ^ 0x8;
        }
    }
}
int solve_phase_4(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_04 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    uint32_t a = (uint32_t)(challenge) & 0x7f;
    uint32_t b = 0;
    uint32_t c = 0x7f;
    uint32_t d = (uint32_t)((challenge>>7) ^ (challenge>>0x20));
    values[0] = p4_walk(a,b,c,d);
    count = 1;
    return write_answer(out, cap, values, count);
}

struct Node{
    uint32_t v0;
    uint32_t v1;
    struct Node *next;
};
static struct Node nodes[11] = {
    {.v0 = 0x00002d02, .v1 = 0x00000527, .next = &nodes[1]},
    {.v0 = 0x0000a7ae, .v1 = 0x00000778, .next = &nodes[2]},
    {.v0 = 0x00009d1b, .v1 = 0x0000060b, .next = &nodes[3]},
    {.v0 = 0x000014c9, .v1 = 0x00000480, .next = &nodes[4]},
    {.v0 = 0x0000aca6, .v1 = 0x00000b53, .next = &nodes[5]},
    {.v0 = 0x00005911, .v1 = 0x00000a17, .next = &nodes[6]},
    {.v0 = 0x0000ec5b, .v1 = 0x00000968, .next = &nodes[7]},
    {.v0 = 0x00005200, .v1 = 0x00000812, .next = &nodes[8]},
    {.v0 = 0x0000825a, .v1 = 0x0000005b, .next = &nodes[9]},
    {.v0 = 0x00004e2c, .v1 = 0x00000376, .next = &nodes[10]},
    {.v0 = 0x0000da9b, .v1 = 0x00000248, .next = NULL},
};
struct Node* head[8] = {
    &nodes[0],
    &nodes[1],
    &nodes[2],
    &nodes[3],
    &nodes[4],
    &nodes[5],
    &nodes[6],
    &nodes[7]
};

uint32_t p5_traverse(uint64_t challenge, uint32_t* out){
    uint32_t index = challenge & 0x7;
    struct Node* node = head[index];
    uint32_t eax = (uint32_t)(challenge >> 0x20) ^ (uint32_t)(challenge >> 0x6);
    eax ^= 0x8011;
    eax = 3*eax + node->v0;
    *out = node->v1;
    eax ^= node->v1;
    uint32_t count = (((uint32_t)challenge)>>3) & 0x3;
    if(count == 0) return eax;
    else{
        for(uint32_t i=1;i<=count;i++){
            node = node->next;
            eax = 3*eax + node->v0;
            eax ^= node->v1;
            *out = node->v1;
        }
    }
    return eax;
}
int solve_phase_5(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_05 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    uint32_t p5_out = 0;
    values[0] = p5_traverse(challenge,&p5_out);
    values[1] = p5_out;
    count = 2;
    return write_answer(out, cap, values, count);
}

uint32_t p6_dispatch(uint64_t challenge){
    uint32_t selector1 = ((uint32_t)challenge) & 0x7;
    uint32_t x = (uint32_t)((challenge >> 0x6) ^ (challenge >> 0x20));
    uint32_t eax = 0;
    switch(selector1){
        case 0: eax = ((x << 5) | (x >> 27)) ^ 0x5f75;
                break;
        case 1: eax =  (2*x + 0xf7c0) ^ (5*x);
                break;
        case 2: eax =  ((x >> 7) ^ x) + 0x8bba;
                break;
        case 3: eax =  (x >> 2) | (x << 30);
                break;
        case 4: eax =  (x+0x5939) ^ (x >> 2);
                break;
        case 5: eax =  (2*x + 0xf7c0) ^ (5*x);
                break;
        case 6: eax =  8*x + (x ^ 0x92c9);
                break;
        case 7: eax =  ((x >> 7) ^ x) & 0x8bba;
                break;
    }
    eax ^= 0x1b3f;
    x = eax;
    uint32_t selector2 = (uint32_t)((challenge >> 3) & 0x7);
    switch(selector2){
        case 0: eax = ((x << 5) | (x >> 27)) ^ 0x5f75;
                break;
        case 1: eax =  (2*x + 0xf7c0) ^ (5*x);
                break;
        case 2: eax =  ((x >> 7) ^ x) + 0x8bba;
                break;
        case 3: eax =  (x >> 2) | (x << 30);
                break;
        case 4: eax =  (x+0x5939) ^ (x >> 2);
                break;
        case 5: eax =  (2*x + 0xf7c0) ^ (5*x);
                break;
        case 6: eax =  8*x + (x ^ 0x92c9);
                break;
        case 7: eax =  ((x >> 7) ^ x) + 0x8bba;
                break;
    }
    return eax + 0x5760;
}
int solve_phase_6(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_06 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    values[0] = p6_dispatch(challenge);
    count = 1;
    return write_answer(out, cap, values, count);
}

#include<string.h>
uint32_t secret_mix(uint32_t x){
    return ((x >> 7) | (x << 25)) ^ 0x8ba3;
}
int solve_secret(uint64_t challenge, uint64_t context, char *out, size_t cap) {
    (void)challenge;
    (void)context;
    (void)out;
    (void)cap;
    // TODO（选做）: 根据 secret_gate、secret_copy 和 secret_phase 的汇编构造完整的第七行，写入 out 后返回 0。
    // 第七行不一定只有整数，可以直接用 snprintf 写入；还没完成时保持返回 1，测试会显示 not attempted。
    uint32_t v0 = (uint32_t) context & 0x3;
    uint32_t arg = (uint32_t)((challenge >> 32) ^ challenge) ^ (uint32_t)((context >> 32) ^ context);
    
    uint32_t key = (uint32_t) v0 * 0x316c;
    key ^= (uint32_t)(context >> 0x10);
    uint32_t v1 = key ^ secret_mix(arg);

    uint32_t r8 = (uint32_t)(context >> 32) ^ (uint32_t)context;
    uint32_t v2 = v1 ^ (r8 + v0 + 0xbacf +((uint32_t)((challenge >> 32) ^ challenge) >> 7));
    
    char buf[256];
    int n = snprintf(buf, sizeof(buf), "%32sOPEN%u %u %u"," ", v0, v1, v2);
    memcpy(out,buf,(size_t)n + 1);
    return 0;
}
