# RC00

Bu proje, istemcilerin birbirleriyle mesajlaşabileceği basit bir **chat uygulaması** sunmaktadır. Uygulama, bir **sunucu** ve birden fazla **istemci** üzerinden çalışır. Sunucu, istemcilere benzersiz bir ID atar, istemciler bu ID'yi kullanarak mesaj gönderebilir. Ayrıca, global sohbet de mümkündür.

## Özellikler
- Sunucu ve istemciler arasında bağlantı sağlanır.
- Her istemciye benzersiz bir ID atanır.
- İstemciler, global chat veya belirli bir ID'ye mesaj gönderebilirler.
- Çoklu istemci desteği, her bir istemci mesajlarını alır ve diğer istemcilere iletebilir.

## Teknolojiler
- **C Programlama Dili**
- **POSIX Threads (pthread)**: Çoklu iş parçacığı kullanılarak mesaj alma ve gönderme işlemleri asenkron yapılır.
- **TCP/IP Soketleri**: Sunucu ve istemci arasındaki iletişim TCP/IP protokolü üzerinden gerçekleştirilir.

## Kullanım

### Sunucu Çalıştırma
```bash
gcc server.c -o server -pthread
./server
```
### İstemci Çalıştırma
```bash
gcc client.c -o client -pthread
./client
./client -p 2
```

## Parametreler
- **Global Sohbet**: ./client
- **Bireysel Mesaj**: ./client -p <ID> şeklinde, belirli bir kullanıcıya mesaj gönderebilirsiniz.


