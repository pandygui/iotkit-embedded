# 阿里云物联网套件

物联网套件是阿里云专门为物联网领域的开发人员推出的，其目的是帮助开发者搭建安全且性能强大的数据通道，方便终端(如传感器，执行器，嵌入式设备或智能家电等等)和云端的双向通信。

# 快速开始

本节描述如何申请自己的设备，并结合本SDK快速体验该设备通过`MQTT`+`TLS/SSL`协议连接到阿里云，上报和接收业务报文。关于SDK的更多使用方式，请访问[官方WiKi](https://github.com/aliyun/iotkit-embedded/wiki)

> 实现原理：
>
> `MQTT协议`(`Message Queuing Telemetry Transport`，消息队列遥测传输)是IBM开发的一个即时通讯协议，是为大量计算能力有限，且工作在低带宽，不可靠的网络的远程传感器和控制设备通讯而设计的协议
>
> 利用MQTT协议是一种基于二进制消息的发布/订阅编程模式的消息协议，下面的应用程序先在阿里云IoT平台订阅(`Subscribe`)一个`Topic`成功, 然后自己向该`Topic`做发布(`Publish`)动作
> 阿里云IoT平台收到之后，就会原样推送回这个应用程序，因为该程序之前已经通过订阅(`Subscribe`)动作成为该`Topic`的一个接收者，发布到这个`Topic`上的任何消息，都会被推送到已订阅该`Topic`的所有终端上

## 一. 开发环境准备

#### **1. 安装Ubuntu16.04**

本SDK的编译环境是`64位`的`Ubuntu16.04`，在其它Linux上尚未测试过，所以推荐安装与阿里开发者一致的发行版

如果您使用`Windows`操作系统，建议安装虚拟机软件`Virtualbox`，下载地址: [https://www.virtualbox.org/wiki/Downloads](https://www.virtualbox.org/wiki/Downloads)

然后安装64位的desktop版本`Ubuntu 16.04.x LTS`，下载地址: [https://www.ubuntu.com/download/desktop](https://www.ubuntu.com/download/desktop)

#### **2. 安装必备软件**

本SDK的开发编译环境使用如下软件: `make-4.1`, `git-2.7.4`, `gcc-5.4.0`, `gcov-5.4.0`, `lcov-1.12`, `bash-4.3.48`, `tar-1.28`, `mingw-5.3.1`

可使用如下命令行安装必要的软件:

    apt-get install -y build-essential make git gcc

#### **3. 下载物联网套件SDK**

当前物联网套件最新SDK版本为Linkkit 2.2.0，可从如下地址获取：
-   **[Linkkit 2.2.0下载地址](https://linkkit-sdk-download.oss-cn-shanghai.aliyuncs.com/linkkit2.2.tar.gz)**

## 二. 在控制台创建设备

#### **1. 注册/登录阿里云账号**

访问阿里云[登录页面](https://account.aliyun.com/login/login.htm), 点击[免费注册](https://account.aliyun.com/register/register.htm), 免费获得一个阿里云账号. 若您已有账号, 可直接登录

#### **2. 访问物联网套件控制台**

登入之后, 鼠标悬停在**产品**上, 弹出层叠菜单，并单击**阿里云Link Platform**

![image](https://linkkit-export.oss-cn-shanghai.aliyuncs.com/%E4%BA%A7%E5%93%81-%E9%98%BF%E9%87%8C%E4%BA%91Link%20Platform.png)

然后, 点击**立即开通**，即可进入IoT控制台

![image](https://linkkit-export.oss-cn-shanghai.aliyuncs.com/%E4%BA%A7%E5%93%81-%E9%98%BF%E9%87%8C%E4%BA%91Link%20Platform-%E7%AB%8B%E5%8D%B3%E5%BC%80%E9%80%9A.png)

#### **3. 创建产品和设备（基础版）**

进入IoT控制台后，点击页面左侧导航栏的**产品管理**，再点击右侧的**创建产品**，如下图所示：

![image](https://linkkit-export.oss-cn-shanghai.aliyuncs.com/LP-%E4%BA%A7%E5%93%81%E7%AE%A1%E7%90%86-%E5%88%9B%E5%BB%BA%E4%BA%A7%E5%93%81.png)

在弹出的创建产品中，选择基础版，填写产品信息：

![image](https://linkkit-export.oss-cn-shanghai.aliyuncs.com/LP-%E4%BA%A7%E5%93%81%E7%AE%A1%E7%90%86-%E5%88%9B%E5%BB%BA%E4%BA%A7%E5%93%81-%E8%AF%A6%E7%BB%86%E5%8F%82%E6%95%B0.png)

填写**产品名称**，选择**节点类型**。
填写好产品信息后，点击**确认**即可生成该产品：
![image](https://linkkit-export.oss-cn-shanghai.aliyuncs.com/LP-%E4%BA%A7%E5%93%81%E7%AE%A1%E7%90%86-%E5%88%9B%E5%BB%BA%E4%BA%A7%E5%93%81-%E7%A1%AE%E8%AE%A4.png)

点击产品右侧的**查看**，可跳转到产品详情页面：
![image](https://linkkit-export.oss-cn-shanghai.aliyuncs.com/LP-%E4%BA%A7%E5%93%81%E7%AE%A1%E7%90%86-%E4%BA%A7%E5%93%81%E8%AF%A6%E6%83%85.png)

在该页面中，有五个主要的选项卡：
- 产品信息：展示产品相关信息，其中ProductKey用于标示产品的品类，该产品下所有设备的ProductKey均一致
- 消息通信：展示产品用于上下行数据的主要Topic
- 服务端订阅：在这里可以选择设备消息类型并推送到MNS队列中，用户服务端从队列里获得设备数据。这样简化服务端订阅设备数据的流程，让客户的服务端能够简单方便并高可靠的获得设备数据。
- 日志服务：此处可浏览设备的历史上下行消息

产品创建好后，接下来可以创建设备了，点击**产品详情**页面中**设备数**旁的**前往管理**，即可看到当前产品下的设备列表，目前为空：

![image](https://linkkit-export.oss-cn-shanghai.aliyuncs.com/LP-%E4%BA%A7%E5%93%81%E7%AE%A1%E7%90%86-%E4%BA%A7%E5%93%81%E8%AF%A6%E6%83%85-%E8%AE%BE%E5%A4%87%E7%AE%A1%E7%90%86.png)

点击上图右侧的**添加设备**，开始创建设备：

![image](https://linkkit-export.oss-cn-shanghai.aliyuncs.com/LP-%E4%BA%A7%E5%93%81%E7%AE%A1%E7%90%86-%E4%BA%A7%E5%93%81%E8%AF%A6%E6%83%85-%E8%AE%BE%E5%A4%87%E7%AE%A1%E7%90%86-%E6%B7%BB%E5%8A%A0%E8%AE%BE%E5%A4%87.png)

在填写好**DeviceName**后，点击确认即可创建该设备，生成设备的**三元组**：
- `ProductKey`：标识产品的品类，相同产品的所有设备ProductKey均相同
- `DeviceName`：标识产品下的每个设备，相同产品的所有设备DeviceName均不相同
- `DeviceSecret`：设备密钥，每个设备均不相同
- **三元组**用于标识阿里云上的每个设备，用于连接阿里云服务器时完成设备认证

![image](https://linkkit-export.oss-cn-shanghai.aliyuncs.com/LP-%E4%BA%A7%E5%93%81%E7%AE%A1%E7%90%86-%E4%BA%A7%E5%93%81%E8%AF%A6%E6%83%85-%E8%AE%BE%E5%A4%87%E7%AE%A1%E7%90%86-%E6%B7%BB%E5%8A%A0%E8%AE%BE%E5%A4%87-%E7%94%9F%E6%88%90%E4%B8%89%E5%85%83%E7%BB%84.png)

至此，基础版产品与设备创建完成。

此外，为了example演示，我们在产品的消息通信中定义一个自定义的topic，用于向服务端发送数据和从服务端接收数据：

![image](https://linkkit-export.oss-cn-shanghai.aliyuncs.com/LP-%E4%BA%A7%E5%93%81-%E6%B6%88%E6%81%AF%E9%80%9A%E4%BF%A1-%E8%87%AA%E5%AE%9A%E4%B9%89topic-data.png)

#### **4. 创建产品和设备（高级版）**

进入IoT控制台后，点击页面左侧导航栏的**产品管理**，再点击右侧的**创建产品**，如下图所示：

![image](https://linkkit-export.oss-cn-shanghai.aliyuncs.com/LP-%E4%BA%A7%E5%93%81%E7%AE%A1%E7%90%86-%E5%88%9B%E5%BB%BA%E4%BA%A7%E5%93%81.png)

在弹出的创建产品中，选择高级版，填写产品信息：

![image](https://linkkit-export.oss-cn-shanghai.aliyuncs.com/LP-ADV-%E4%BA%A7%E5%93%81%E7%AE%A1%E7%90%86-%E5%88%9B%E5%BB%BA%E4%BA%A7%E5%93%81-%E8%AF%A6%E7%BB%86%E5%8F%82%E6%95%B0.png)
在此需要填写以下信息：

- 填写**产品名称**
- 选择**节点类型**
- 选择**设备类型**，如果没有需要的设备类型，可以填空，稍后在产品**功能定义**时再定义具体的功能
- 选择**数据格式**，目前有**Alink JSON**和**透传/自定义**两种格式可选（在这里我们使用Alink JSON格式作为示例）  
    **Alink JSON**：使用标准的Alink协议格式来进行物联网套件与云端的服务、属性、事件的数据交换  
    **透传/自定义**：使用物联网套件的透传接口来进行与云端的数据交换，在云端需要用户完成透传数据与Alink格式的转换脚本
- **ID2**：是否使用ID2认证

填写好产品信息后，点击**确认**即可生成该产品：

![image](https://linkkit-export.oss-cn-shanghai.aliyuncs.com/LP-ADV-%E4%BA%A7%E5%93%81%E7%AE%A1%E7%90%86-%E5%88%9B%E5%BB%BA%E4%BA%A7%E5%93%81-%E7%A1%AE%E8%AE%A4.png)

点击产品右侧的**查看**，可跳转到产品详情页面：

![image](https://linkkit-export.oss-cn-shanghai.aliyuncs.com/LP-ADV-%E4%BA%A7%E5%93%81%E7%AE%A1%E7%90%86-%E4%BA%A7%E5%93%81%E8%AF%A6%E6%83%85.png)

在该页面中，有五个主要的选项卡：
- 产品信息：展示产品相关信息，其中ProductKey用于标示产品的品类，该产品下所有设备的ProductKey均一致
- 消息通信：展示产品用于上下行数据的主要Topic
- 功能定义：在这里可以定义设备的物模型，定义物的服务、属性、事件
- 日志服务：此处可浏览设备的历史上下行消息
- 在线调试：此处可对该产品下的设备进行在线调试，如下发服务到设备，设置设备的属性，观察设备的事件上报

产品创建好后，接下来可以创建设备了，点击**产品详情**页面中**设备数**旁的**前往管理**，即可看到当前产品下的设备列表，目前为空：

![image](https://linkkit-export.oss-cn-shanghai.aliyuncs.com/LP-ADV-%E4%BA%A7%E5%93%81%E7%AE%A1%E7%90%86-%E4%BA%A7%E5%93%81%E8%AF%A6%E6%83%85-%E8%AE%BE%E5%A4%87%E7%AE%A1%E7%90%86.png)

点击上图右侧的**添加设备**，开始创建设备：

![image](https://linkkit-export.oss-cn-shanghai.aliyuncs.com/LP-ADV-%E4%BA%A7%E5%93%81%E7%AE%A1%E7%90%86-%E4%BA%A7%E5%93%81%E8%AF%A6%E6%83%85-%E8%AE%BE%E5%A4%87%E7%AE%A1%E7%90%86-%E6%B7%BB%E5%8A%A0%E8%AE%BE%E5%A4%87.png)

在填写好**DeviceName**后，点击确认即可创建该设备，生成设备的**三元组**：
- `ProductKey`：标识产品的品类，相同产品的所有设备ProductKey均相同
- `DeviceName`：标识产品下的每个设备，相同产品的所有设备DeviceName均不相同
- `DeviceSecret`：设备密钥，每个设备均不相同
- **三元组**用于标识阿里云上的每个设备，用于连接阿里云服务器时完成设备认证

![image](https://linkkit-export.oss-cn-shanghai.aliyuncs.com/LP-ADV-%E4%BA%A7%E5%93%81%E7%AE%A1%E7%90%86-%E4%BA%A7%E5%93%81%E8%AF%A6%E6%83%85-%E8%AE%BE%E5%A4%87%E7%AE%A1%E7%90%86-%E6%B7%BB%E5%8A%A0%E8%AE%BE%E5%A4%87-%E7%94%9F%E6%88%90%E4%B8%89%E5%85%83%E7%BB%84.png)

至此，高级版产品与设备创建完成。

稍后会展示高级版服务、属性、事件的示例程序，所以在这里我们回到**产品管理**，选择我们刚才创建的产品，进入**产品详情**页，选择**功能定义**选项卡：

![image](https://linkkit-export.oss-cn-shanghai.aliyuncs.com/LP-ADV-%E4%BA%A7%E5%93%81%E7%AE%A1%E7%90%86-%E4%BA%A7%E5%93%81%E8%AF%A6%E6%83%85-%E5%8A%9F%E8%83%BD%E5%AE%9A%E4%B9%89.png)

选择右侧的**新增**，我们先创建一个属性：  
该属性是一个字符串属性，最大长度为2048个字节

![image](https://linkkit-export.oss-cn-shanghai.aliyuncs.com/LP-ADV-%E4%BA%A7%E5%93%81%E7%AE%A1%E7%90%86-%E4%BA%A7%E5%93%81%E8%AF%A6%E6%83%85-%E5%8A%9F%E8%83%BD%E5%AE%9A%E4%B9%89-%E6%96%B0%E5%A2%9E%E5%AD%97%E7%AC%A6%E4%B8%B2%E5%B1%9E%E6%80%A7.png)
点击**确认**，这样一个属性就创建好了

![image](https://linkkit-export.oss-cn-shanghai.aliyuncs.com/LP-ADV-%E4%BA%A7%E5%93%81%E7%AE%A1%E7%90%86-%E4%BA%A7%E5%93%81%E8%AF%A6%E6%83%85-%E5%8A%9F%E8%83%BD%E5%AE%9A%E4%B9%89-%E6%96%B0%E5%A2%9E%E5%AD%97%E7%AC%A6%E4%B8%B2%E5%B1%9E%E6%80%A7-%E5%88%9B%E5%BB%BA%E6%88%90%E5%8A%9F.png)

接下来我们创建一个服务：  
关于服务的创建要注意的是，服务只能由服务端下发，设备端被动接收，每个服务有自己的输入参数和输出参数，说明如下：  
- 服务的**输入参数**：指的是当从云端下发服务时，云端向设备端发送的数据内容
- 服务的**输出参数**：指的是当设备端收到从云端下发的服务时，如果有需要对该服务返回一些业务数据，那么就是输出参数的内容  
在这个服务中我们创建一个输入参数和一个输出参数

![image](https://linkkit-export.oss-cn-shanghai.aliyuncs.com/LP-ADV-%E4%BA%A7%E5%93%81%E7%AE%A1%E7%90%86-%E4%BA%A7%E5%93%81%E8%AF%A6%E6%83%85-%E5%8A%9F%E8%83%BD%E5%AE%9A%E4%B9%89-%E6%96%B0%E5%A2%9E%E6%9C%8D%E5%8A%A1.png)

最后，再创建一个事件：
关于事件要注意的是，事件只能由设备端主动上报给服务端，每个事件只有输出参数，表示从设备端需要上报的数据内容  
在这个事件中我们创建一个输出参数

![image](https://linkkit-export.oss-cn-shanghai.aliyuncs.com/LP-ADV-%E4%BA%A7%E5%93%81%E7%AE%A1%E7%90%86-%E4%BA%A7%E5%93%81%E8%AF%A6%E6%83%85-%E5%8A%9F%E8%83%BD%E5%AE%9A%E4%B9%89-%E6%96%B0%E5%A2%9E%E4%BA%8B%E4%BB%B6.png)

至此，我们创建的产品中服务、属性、事件各有一个：

![image](https://linkkit-export.oss-cn-shanghai.aliyuncs.com/LP-ADV-%E4%BA%A7%E5%93%81%E7%AE%A1%E7%90%86-%E4%BA%A7%E5%93%81%E8%AF%A6%E6%83%85-%E5%8A%9F%E8%83%BD%E5%AE%9A%E4%B9%89-%E5%AE%8C%E6%88%90.png)


## 三. 编译运行样例程序

#### **1. 样例程序（基础版）**


#### **1.1 填入设备信息**

编辑文件`iotx-sdk-c/examples/mqtt/mqtt-example.c`, 编辑如下代码段, 填入之前**创建产品和设备**步骤中得到的**设备三元组**:

![image](https://linkkit-export.oss-cn-shanghai.aliyuncs.com/LP-%E4%BB%A3%E7%A0%81%E7%A4%BA%E4%BE%8B-mqtt-example-%E4%B8%89%E5%85%83%E7%BB%84%E4%BF%AE%E6%94%B9.png)

#### **1.2 编译SDK产生样例程序**

运行如下命令:

    $ make distclean
    $ make

编译成功完成后, 生成的样例程序在当前目录的`output/release/bin`目录下:

    $ tree output/release
    output/release/
    ├── bin
    │   ├── http-example
    │   ├── linkkit-example-sched
    │   ├── linkkit-example-solo
    │   ├── linkkit_tsl_convert
    │   ├── mqtt-example
    │   ├── mqtt_multi_thread-example
    │   ├── mqtt_rrpc-example
    │   ├── ota_mqtt-example
    │   ├── sdk-testsuites
    │   └── uota_app-example
    ...
    ...

#### **1.3 执行样例程序**

    $ ./output/release/bin/mqtt-example 
    [inf] iotx_device_info_init(39): device_info created successfully!
    [dbg] iotx_device_info_set(49): start to set device info!
    [dbg] iotx_device_info_set(63): device_info set successfully!
    [inf] guider_print_dev_guider_info(279): ....................................................
    [inf] guider_print_dev_guider_info(280):           ProductKey : a1ExpAkj9Hi
    [inf] guider_print_dev_guider_info(281):           DeviceName : Example1
    [inf] guider_print_dev_guider_info(282):             DeviceID : a1ExpAkj9Hi.Example1
    [inf] guider_print_dev_guider_info(284): ....................................................
    [inf] guider_print_dev_guider_info(285):        PartnerID Buf : ,partner_id=example.demo.partner-id
    [inf] guider_print_dev_guider_info(286):         ModuleID Buf : ,module_id=example.demo.module-id
    [inf] guider_print_dev_guider_info(287):           Guider URL : 
    [inf] guider_print_dev_guider_info(289):       Guider SecMode : 2 (TLS + Direct)
    [inf] guider_print_dev_guider_info(291):     Guider Timestamp : 2524608000000
    [inf] guider_print_dev_guider_info(292): ....................................................
    [inf] guider_print_dev_guider_info(298): ....................................................
    [inf] guider_print_conn_info(256): -----------------------------------------
    [inf] guider_print_conn_info(257):             Host : a1ExpAkj9Hi.iot-as-mqtt.cn-shanghai.aliyuncs.com
    [inf] guider_print_conn_info(258):             Port : 1883
    [inf] guider_print_conn_info(261):         ClientID : a1ExpAkj9Hi.Example1|securemode=2,timestamp=2524608000000,signmethod=hmacsha1,gw=0,ext=0,partner_id=example.demo.partner-id,module_id=example.demo.module-id|
    [inf] guider_print_conn_info(263):       TLS PubKey : 0x437636 ('-----BEGIN CERTI ...')
    [inf] guider_print_conn_info(266): -----------------------------------------
    [inf] IOT_MQTT_Construct(3005):      CONFIG_MQTT_SUBTOPIC_MAXNUM : 65535
    [dbg] IOT_MQTT_Construct(3007): sizeof(iotx_mc_client_t) = 1573144!
    [inf] iotx_mc_init(2098): MQTT init success!
    [inf] _ssl_client_init(142): Loading the CA root certificate ...
    [inf] _ssl_client_init(149):  ok (0 skipped)
    [inf] _TLSConnectNetwork(315): Connecting to /a1ExpAkj9Hi.iot-as-mqtt.cn-shanghai.aliyuncs.com/1883...
    [inf] mbedtls_net_connect_timeout(257): setsockopt SO_SNDTIMEO timeout: 10s
    [inf] mbedtls_net_connect_timeout(260): connecting IP_ADDRESS: 106.15.100.2
    [inf] _TLSConnectNetwork(328):  ok
    [inf] _TLSConnectNetwork(333):   . Setting up the SSL/TLS structure...
    [inf] _TLSConnectNetwork(343):  ok
    [inf] _TLSConnectNetwork(382): Performing the SSL/TLS handshake...
    [inf] _TLSConnectNetwork(390):  ok
    [inf] _TLSConnectNetwork(394):   . Verifying peer X.509 certificate..
    [inf] _real_confirm(90): certificate verification result: 0x00
    [wrn] MQTTConnect(204): NOT USING pre-malloced buf 0xc4a010, malloc per packet
    [dbg] MQTTConnect(204): ALLOC: curr buf = 0xc56890, curr buf_size = 320, required payload_len = 256
    [dbg] MQTTConnect(224): FREED: curr buf = (nil), curr buf_size = 0
    [inf] iotx_mc_connect(2449): mqtt connect success!
    ...
    ...
    mqtt_client|309 :: packet-id=7, publish topic msg={"attr_name":"temperature", "attr_value":"1"}
    [dbg] iotx_mc_cycle(1591): PUBACK
    event_handle|132 :: publish success, packet-id=7
    [dbg] iotx_mc_cycle(1608): PUBLISH
    [dbg] iotx_mc_handle_recv_PUBLISH(1412):         Packet Ident : 00035641
    [dbg] iotx_mc_handle_recv_PUBLISH(1413):         Topic Length : 26
    [dbg] iotx_mc_handle_recv_PUBLISH(1417):           Topic Name : /a1ExpAkj9Hi/Example1/data
    [dbg] iotx_mc_handle_recv_PUBLISH(1420):     Payload Len/Room : 45 / 992
    [dbg] iotx_mc_handle_recv_PUBLISH(1421):       Receive Buflen : 1024
    [dbg] iotx_mc_handle_recv_PUBLISH(1432): delivering msg ...
    [dbg] iotx_mc_deliver_message(1170): topic be matched
    _demo_message_arrive|166 :: ----
    _demo_message_arrive|167 :: packetId: 35641
    _demo_message_arrive|171 :: Topic: '/a1ExpAkj9Hi/Example1/data' (Length: 26)
    _demo_message_arrive|175 :: Payload: '{"attr_name":"temperature", "attr_value":"1"}' (Length: 45)
    _demo_message_arrive|176 :: ----
    ...
    ...
    main|361 :: out of sample! 

#### **1.4 观察消息上报**

如下日志信息显示样例程序正在通过`MQTT`的`Publish`类型消息, 上报业务数据到`/${prodcutKey}/${deviceName}/data`

    mqtt_client|309 :: packet-id=7, publish topic msg={"attr_name":"temperature", "attr_value":"1"}

#### **1.5 观察消息下推**

如下日志信息显示该消息因为是到达已被订阅的`Topic`, 所以又被服务器原样推送到样例程序, 并进入相应的回调函数

    _demo_message_arrive|166 :: ----
    _demo_message_arrive|167 :: packetId: 35641
    _demo_message_arrive|171 :: Topic: '/a1ExpAkj9Hi/Example1/data' (Length: 26)
    _demo_message_arrive|175 :: Payload: '{"attr_name":"temperature", "attr_value":"1"}' (Length: 45)
    _demo_message_arrive|176 :: ----

#### **1.6 观察控制台日志**

可以登录物联网套件控制台, 到**产品管理**, 找到刚才创建的产品，点击**查看**，选择**日志服务**选项卡，可以看到刚才上报的消息（Message ID为7）

![image](https://linkkit-export.oss-cn-shanghai.aliyuncs.com/LP-%E4%BA%A7%E5%93%81%E7%AE%A1%E7%90%86-%E4%BA%A7%E5%93%81%E8%AF%A6%E6%83%85-%E6%97%A5%E5%BF%97%E6%9C%8D%E5%8A%A1.png)

#### **2. 样例程序（高级版）**


#### **2.1 填入设备信息**

# 关于SDK的更多使用方式, 请访问[阿里云物联网平台帮助文档](https://help.aliyun.com/product/30520.html?spm=5176.11485173.0.0.534659af6BLaB7)