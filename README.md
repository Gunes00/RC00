# RC00

Bu proje, istemcilerin birbirleriyle mesajlaşabileceği basit bir **chat uygulaması** sunmaktadır. Uygulama, bir **sunucu** ve birden fazla **istemci** üzerinden çalışır. Sunucu, istemcilere benzersiz bir ID atar, istemciler bu ID'yi kullanarak mesaj gönderebilir.

## Özellikler
- Sunucu ve istemciler arasında bağlantı sağlanır.
- Her istemciye benzersiz bir ID atanır.
- Çoklu istemci desteği, her bir istemci mesajlarını alır ve diğer istemcilere iletebilir.

## Teknolojiler
- **C Programlama Dili**
- **POSIX Threads (pthread)**: Çoklu iş parçacığı kullanılarak mesaj alma ve gönderme işlemleri asenkron yapılır.
- **TCP/IP Soketleri**: Sunucu ve istemci arasındaki iletişim TCP/IP protokolü üzerinden gerçekleştirilir.

## Kullanım

### Kurulum
```bash
make all
```

### Sunucu Çalıştırma
```bash
./server
```

### İstemci Çalıştırma
```bash
./client 'Nickname'
```

### Kaldırmak İçin
```bash
make clean
```

