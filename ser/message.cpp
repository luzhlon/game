#include "message.h"

void PackageCache::write_data(char *buf, int size) {

    write(buf, size);

    //处理完已经收到的包
    while(true) {
        if(delta() < sizeof(msg_len))
            break;

        msg_len pkg_len = *(msg_len *)(&m_buf[i_read]);
        if(delta() >= pkg_len) {
            m_cb(&m_buf[i_read], pkg_len);
            i_read += pkg_len;
        } else {
            break;
        }
    }

    //将剩余的没处理的数据Copy到前面
    int rest_len = delta();
    for(int i = 0; i < rest_len; i++) { m_buf[i] = m_buf[i + rest_len]; }
    i_write -= i_read;
    i_read = 0;
}
