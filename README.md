# Netease Lyric Dump
### 这是干啥的
可能你和我一样——只把网易云作为音乐下载器，下好了之后用别的播放器播放。但又苦于这些播放器没有歌词匹配功能，又或是匹配到的歌词对不上。而你又懒得去翻网易云的歌词文件再把它改成正常的.lyc文件——于是就有了这个东西
### 注意

1. 获取歌曲名需要联网
1. **既然可以获取歌曲名，为什么不直接在列表里显示歌曲名而是显示歌曲ID：**短时间向网易云的服务器发送过多请求会被当成恶意攻击而被ban，届时返回不到歌曲名，解ban要么重启要么换网络要么等30分钟

### <span id=已知问题>已知问题</span>

1. 先连接网络读取一次歌词，再关闭网络进行一次读取，会报错 `std::bad_alloc` 然后崩溃

### TODO

1. 用Qt Quick重写这个ugly至极的ui
1. 修复[已知问题](#已知问题)