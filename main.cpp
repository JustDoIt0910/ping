#include <iostream>
#include "ping.h"

int main(int argc, char *argv[]) {
    bool flag = false; // �����ж��Ƿ�ʹ�ò��� -t
    if (argc < 2) {
        std::cout << "�÷���ping [-t] target_name" << std::endl;
        std::cout << "ѡ�-t \t Ping ָ����������ֱ��ֹͣ��" << std::endl;
        exit(0);
    }
    else {
        if (std::string(argv[1]) == "-t")
            flag = true;
    }

    MyPing my_ping;

    // ��������
    std::string cname;
    cname = flag ? argv[2] : argv[1];
    struct hostent *host = gethostbyname(cname.c_str());
    if (!host) {
        printf("Ping �����Ҳ������� %s����������ƣ�Ȼ�����ԡ�", cname.c_str());
        exit(0);
    }
    const char *ipaddr = inet_ntoa(*(struct in_addr *)host->h_addr_list[0]); // ip��ַ

    Reply reply{};
    EndInfo info{0, 0, 0, 9999};

    printf("���� Ping %s [ %s ] ���� %d �ֽڵ�����\n", cname.c_str(), ipaddr, DEF_PACKET_SIZE);
    int i = 4;
    while (flag || i--) { // ���ö�·ЧӦʵ��-t����
        info.sent_num++;
        // ����Ping�����ж��Ƿ�ʱ
        if (my_ping.Ping(ipaddr, &reply)) {
            printf("���� %s �Ļظ��� �ֽ�=%d ʱ��=%ldms TTL=%ld\n", ipaddr, reply.bytes, reply.rtt, reply.ttl);
            info.min_rtt = std::min<USHORT>(info.min_rtt, reply.rtt);
            info.max_rtt = std::max<USHORT>(info.max_rtt, reply.rtt);
            info.recv_num++;
        }
        else {
            printf("����ʱ��\n");
        }
        Sleep(1000);
    }

    // ping���������չʾpingͳ����Ϣ
    printf("\n%s �� Ping ͳ����Ϣ:\n", ipaddr);
    printf("\t���ݰ�: �ѷ��� = %d���ѽ��� = %d����ʧ = %d��\n", info.sent_num, info.recv_num, info.sent_num - info.recv_num);
    // ֻ�����յ�1�����ݰ������ϲ�չʾ������Ϣ
    if (info.recv_num > 1) {
        printf("�����г̵Ĺ���ʱ��(�Ժ���Ϊ��λ):\n");
        printf("\t��� = %dms��� = %dms��ƽ�� = %dms\n", info.min_rtt, info.max_rtt, (info.min_rtt + info.max_rtt) / 2);
    }

    return 0;
}