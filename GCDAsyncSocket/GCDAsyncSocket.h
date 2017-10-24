//
//  GCDAsyncSocket.h
//  CocoaAsyncSocket
//
//  Created by tamer on 2017/10/18.
//  Copyright © 2017年 tamer. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface GCDAsyncSocket : NSObject

- (id)init;
- (id)initWithSocketQueue:(dispatch_queue_t)sq;
- (id)initWithDelegate:(id)aDelegate delegateQueue:(dispatch_queue_t)dq;
- (id)initWithDelegate:(id)aDelegate delegateQueue:(dispatch_queue_t)dq socketQueue:(dispatch_queue_t)sq;

#pragma mark - Configuration

@property (atomic, weak, readwrite) id delegate;
#if OS_OBJECT_USE_OBJC
@property (atomic, strong, readwrite) dispatch_queue_t delegateQueue;
#else
@property (atomic, assign, readwrite) dispatch_queue_t delegateQueue;
#endif

- (void)getDelegate:(id *)delegatePtr delegateQueue:(dispatch_queue_t *)delegateQueuePtr;
- (void)setDelegate:(id)delegate delegateQueue:(dispatch_queue_t)delegateQueue;

/**
 * If you are setting the delegate to nil within the delegate's dealoc method,
 * you may need to use the synchronous versions below.
 * 如果你需要在代理的dealloc方法中把代理置空，请使用以下的方法。
 */
- (void)synchronouslySetDelegate:(id)delegate;
- (void)synchronouslySetDelegateQueue:(dispatch_queue_t)delegateQueue;
- (void)synchronouslySetDelegate:(id)delegate delegateQueue:(dispatch_queue_t)delegateQueue;

/**
 * By default, both IPv4 and IPv6 are enabled.
 * 默认支持IPv4 & IPv6 两种协议。
 *
 * For accepting incoming connections, this means GCDAsyncSocket automatically supports both protocols,
 * and can simulataneously accept incoming on either protocol.
 * GCDAsyncSocket可接受的请求链接，自动支持两种协议，也可以模拟接受两者间任意一种协议的连接请求。
 *
 *  For outgoning connecting, this means GCDasyncSocket can connect to remote hosts running either protocol.
 *  If a DNS lookup returns only IPv4 results, GCDAsyncSocket will automatically use IPv4.
 *  If a DNS lookup returns only IPv6 results, GCDAsyncSocket will automatically use IPv6.
 *  If a DNS looup returns IPv4 and IPv6 results, the preferred protocol wwill bo chosen.
 *  By default, the preferred protocol is IPv4, but may be configured as desired.
 *  GCDAsyncSocket 可以发起两者间任意一种协议的连接请求到远端。
 *  如果DNS解析只返回IPv4，则自动使用IPv4协议。
 *  如果DNS解析只返回IPv6，则自动使用IPv6协议。
 *  默认的优选IPv4，你也可以配置。
 */
@property (atomic, assign, readwrite, getter=isIPv4Enabled) BOOL IPv4Enabled;
@property (atomic, assign, readwrite, getter=isIPv6enabled) BOOL IPv6Enabled;
@property (atomic, assign, readwrite, getter=isIPv4PreferredOverIPv6) BOOL IPv4PreferredOverIPv6;

/**
 *  User data allows you to associate arbitrary information with the socket.
 *  This data is not use internally by socket in any way.
 *  可以为socket指定任何信息的user data。
 *  指定的信息，不再socket内部使用。
 */
@property (atomic, strong) id userData;

#pragma mark - Accepting

/**
 *  Tells the socket to begin listening and accept connections on the given port.
 *  When a connection is accpet, a new instance of GCDAsyncSocket will be spawned to handle it,
 *  and the socket:didAccpetNewSocket: delegate method will be invoked.
 *
 *  The socket will listen on all available interfaces (e.g. wifi, ethent, etc)
 *
 *  socket监听，接受指定端口的连接请求。
 *  当连接被接受时，会创建一个GCDAsyncSocket实例去处理该socket连接，
 *  并调用socket:didAccpetNewSocket:代理方法。
 *
 *  套接字将会监听所有可用的接口(如wifi、ethent等)
 */
- (BOOL)accpetOnPort:(uint16_t)port error:(NSError **)errPtr;

/**
 *  This method is the same as accpetOnPort:error wtih the additional option of specifying which interface to listen on.
 *
 *  For example, you could specify that the socket should only accpet connections over etherent,
 *  and not other interface such as wifi.
 *
 *  The interface mey be specify by name (e.g. "en1" or "lo0") or by IP address (e.g. "192.168.4.34").
 *  You may also use the specify strings "localhost" or "lookback" to specify that socket only accpet connections from
 *  local machine.
 *
 *  该方法与  accpetOnPort:error 方法类似。
 *
 *  你可以指定socket只适用于以太连接，不接受类似wifi的其他接口。
 *
 *  可以通过名字（例如：“en1”或“lo0”）或者IP地址（e.g. “192.168.4.34”）来指定接口。
 *  你可以通过 "localhost" 或 "lookback"来指明该socket只接受本地机器的连接。
 */
- (BOOL)acceptOnInterface:(NSString *)interface port:(uint16_t)port error:(NSError **)errPtr;

#pragma mark - connecting

/**
 *  Connects to the given host and port.
 *
 *  This method invokes connectToHost:onPort:viaInterface:withTimeout:error:
 *  and uses the default inteface, and no timeout.
 *
 *  连接到指定的主机和端口。
 *
 *  这个方法会调用 connectToHost:onPort:viaInterface:withTimeout:error:，使用默认的接口且不设置超时时间。
 */
- (BOOL)connectToHost:(NSString *)host onPort:(uint16_t)port error:(NSError **)errptr;

/**
 *  Connects to the given host and port with an optional timeout.
 *
 *  This method invokes connectToHost:onPort:viaInterface:withTimeout:error: and uses the default interface.
 *
 *  以可选的超时时间连接到指定的主机端口。
 *  该方法会调用 connectToHost:onPort:viaInterface:withTimeout:error:，并使用默认接口。
 */
- (BOOL)connectToHost:(NSString *)host onPort:(uint16_t)port withTimeout:(NSTimeInterval)timeout error:(NSError **)errPtr;

/**
 *  Connects to the given host and port, via the optional interface, with an optional timeout.
 *
 *  The host may be domain name (e.g. "deusty.com") or an IP address string (e.g. "192.168.0.2").
 *  The host may also be the special striongs "localhost" or "lookback" to specify connecting to a service on the local machine.
 *
 *  The interface may be a name (e.g. "en1" or "lo0") or the corresponding IP address (e.g. "192.168.4.35").
 *  The interface may also be used to specify the local port (see below).
 *
 *  To not time out use a negative time interval.
 *
 *  This method will return NO if an error is detected, and set the error pointer (if one was given).
 *   
 *  Possible errors would be a nil host, invalid interface, or socket is already connected.
 *
 *  If no errors are detected, this method will start a background connect operation and immediately return YES.
 *  The delegate callbacks are used to notify you when the socket connects, or if the host was unreachable.
 *
 *  Since this class supports queued reads and writes, you can immediately start reading and/or writing
 *  All read/write operations will be queued, and upon socket connecttion, the operations will be dequeued and processed in order.
 *
 *  The inteface may optionally contain a port number at the end of string, separated by a colon.
 *  This allows you to specify the local port that should be used for the outgoing connection.
 *  To specify both interface and the local port:"en1:8082" or "192.168.4.35:2424".
 *  To specify only local port:":8082".
 *  Please note this is an advanced feature, and is somewhat hidden on purpose.
 *  You should understand that 99.99% of the time you should NOT specify the loacl port for an outgoing connection.
 *  If you think you need to, there is a good chance you have a fundamental misunderstanding somewhere.
 *  Local port do NOT need to match reomte ports. In fact, they almost nerver do.
 *  This feature is here for networking professionals using very advanced techniques.
 *
 *  以可选的接口超时时间连接到指定的主机端口。
 *  主机可以指定为域名或者IP地址。
 *  主机也可以通过"localhost" 或者 "lookback" 字符串指定为本地机器服务
 *  接口可以通过名字或相应的IP地址来指定。
 *  接口也可以通过本地端口来指定。
 *  负的时间间隔代表没有超时。
 *  当发生错误时，该方法会返回NO，并对将error信息传递给error指针。
 *  主机为空，无效的接口，或者套接字已经被连接都可能导致错误。
 *  如果没有检测到错误，该方法会开启一个后台线程去执行连接操作，并立即返回YES。
 *  socket 连接成功或着主机无法访问时，会通过代理回调给你。
 *  所有的读写操作都可以进入操作队列，或从socket 连接上的队列中移除，并进一步处理。
 *  在inteface参数的末尾可以包含一个端口号，以冒号隔开。
 *  该参数允许你为外部链接指定一个本地端口。
 *  可以这样指定inteface和端口："en1:8082" or "192.168.4.35:2424".
 *  也可以只指定本地端口：":8082".
 *  请注意这是一个高级的功能，并且在某种程度上是故意隐藏的。
 *  如果你认为你需要，很可能你在某个地方有一个根本性的误解。
 *  这个特性是为使用非常高级技术的网络专业人员提供的。
 */
- (BOOL)connectToHost:(NSString *)host
               onPort:(uint16_t)port
         viaInterface:(NSString *)interface
          withTimeout:(NSTimeInterval)timout
                error:(NSError **)errPtr;

/**
 *  Connects to the unix domain socket at the given URL, using the specified timout.
 *  使用指定的timout连接到给定URL的unix域套接字。
 */
- (BOOL)connectToURL:(NSURL *)URL withTimeout:(NSTimeInterval)timeout error:(NSError **)error;

#pragma mark - Disconnecting

/**
 *  Disconnects immediately (synchronously). Any pending reads or writes are dropped.
 *  
 *  If the socket is not already disconnected, an invocation to the socketDidDisconnecte:withError: delegate method 
 *  will be queued onto the delegateQueue asynchronously (behind any previously queued delegate methods).
 *  In other words, the disconnected delegate method will be invoke sometime shortly after this method returns.
 *
 *  Please note the recommended way of releasing a GCDAsyncSocket instance (e.g. in a dealloc method).
 *  [asyncSocket setDelegate:nil];
 *  [asyncSocket disconnect];
 *  [asyncSocket release];
 *
 *  If you plan on disconnecting the socket, and then immediately asking it to connect again.
 *  You'll likely  want to do so like this:
 *  [asyncSocket setDelegate:nil];
 *  [asyncSocket disconnect];
 *  [asyncSocket setDelegate:self];
 *  [asyncSocket connect...];
 *
 *  立即断开连接（同步的）。 任何未开始的读写操作都会被删除。
 *  
 *  连接未能断开时，socketDidDisconnecte:withError:代理方法会被加入到异步队列执行。
 *  也就是说，该方法调用后，很快就会调用相应的代理方法。
 *
 *  推荐使用以下方式去释放一个GCDAsyncSocket实例。
 *  [asyncSocket setDelegate:nil];
 *  [asyncSocket disconnect];
 *  [asyncSocket release];
 *
 *  若果你打算立即重新连接一个正在断开的socket连接，可以使用以下方式：
 *  [asyncSocket setDelegate:nil];
 *  [asyncSocket disconnect];
 *  [asyncSocket setDelegate:self];
 *  [asyncSocket connect...];
 */
- (void)disconnect;

/**
 *  Disconnects after all pending reads have complete.
 *  After calling this, the read and write methods will do nothing.
 *  The socket will be disconnect even if there are still pending writes.
 *  
 *  执行完所有未执行的读取操作后断开socket连接。
 *  当调用该方法，读写方法将不再执行任何操作。
 *  即使还有未完成写入操作，socket连接也会断开。
 */
- (void)disconnectAfterReading;

/**
 *  Disconnects after all pending writes have complete.
 *  After calling this, the read and write methods will be do nothing.
 *  the socket will be disconnect even if there are still pending reads.
 *
 *  执行完所有未执行的写入操作后断开socket连接。
 *  当调用该方法，读写方法将不再执行任何操作。
 *  即使还有未完成读取操作，socket连接也会断开。
 */
- (void)disconnectAfterWriting;

/**
 *  Disconnect after all pending reads and writes have complete.
 *  After calling this, the read and write method will be do nothing.
 *
 *  执行完所有未执行的读写操作后断开socket连接。
 *  当调用该方法，读写方法将不再执行任何操作。
 */
- (void)disconnectAfterReadingAndWriting;

#pragma mark - Diagnostics

/**
 *  Returns whether the socket is disconnected or connected.
 *  
 *  A disconnected socket may be recycled.
 *  That is, it can be used again for connecting or listening.
 *
 *  If a socket is in the process of connecting, it may be neither disconnected nor connected.
 *
 *  返回socket断开或连接的状态。
 *  一个断开的socket也可以被复用。
 *  意味着，这个socket可以被重连或重新监听。
 *  如果一个socket正在连接中，则它的状态可能不是断开也不是连接。
 */
@property (atomic, readonly) BOOL isDisconnected;
@property (atomic, readonly) BOOL isConnected;

/**
 *  Returns the local or remote host and port to which this socket is connected, or nil and 0 if not connected.
 *  The host will be an IP address.
 *
 *  返回对应socket的本地或远端的host和port。
 *  如果socket未连接，则返回nil和0。
 *  host有可能是IP地址。
 */
@property (atomic, readonly, nullable) NSString *connectedHost;
@property (atomic, readonly) uint16_t connectedPort;
@property (atomic, readonly, nullable) NSString *connectedURL;

@property (atomic, readonly, nullable) NSString *localHost;
@property (atomic, readonly) uint16_t localPort;


/**
 *  Returns the local or remote address to which this socket is connected,
 *  specified as a sockaddr structure wrapped in a NSData oject.
 *  @seealso connectedHost
 *  @seealso connectedPort
 *  @seealso localHost
 *  @seealso localPort
 */
@property (atomic, readonly, nullable) NSData *connectedAddress;
@property (atomic, readonly, nullable) NSData *localAddress;

/**
 *  Returns whether the socket is IPv4 or IPv6.
 *  An accepting socket may be both.
 */
@property (atomic, readonly) BOOL isIPv4;
@property (atomic, readonly) BOOL isIPv6;

/**
 *  Returns whether or not the socket has been secured via TSL/SSL.
 *
 *  套接字是否经过TSL／SSL加密。
 *
 *  See also the startTLS method.
 */
@property (atomic, readonly) BOOL isSecure;

#pragma mark - Reading

/**
 *  The readData and writeData methods won't block (they are asynchronously).
 *
 *  When a read is complete the socket:didReadData:withTag: delegate method is dispatched on the delegateQueue.
 *  When a write is complete the socket:didWriteData:withTag: delegate method is dispatched on the delegateQueue.
 *
 *  You may optionally set a timeout for any read/write operation. (To not timeout, use a negative time interval.)
 *  If a read/write operation time out, the corresponding "socket:shouldTimeout.." delegate method 
 *  is called to optionally allow you to extend the timeout.
 *  Upon a timeout, the "socket:didDisconnectedWtihError:" method is called
 *
 *  The tag is for your convenience.
 *  You can use it as an array index, step number, state id, pointer, etc.
 *
 *  readData和writeData方法都不会阻塞，它们都是异步的。
 *  当一个读操作完成后就会通过代理队列回调socket:didReadData:withTag:代理方法。
 *  当一个写操作完成后就会通过代理队列回调socket:didWriteData:withTag:代理方法。
 *  你可以为读写操作设置超时时间。（不使用超时，可以指定一个负数）
 *  如果一个读写操作超时了，对应的 "socket:shouldTimeout.."代理方法会被调用，这时你有机会延长超时时间。
 *  Tag可以作为数组的index、步骤的编号、状态码、指针等等。
 *
 */

/**
 *  Read the first available bytes that become available on the socket.
 *
 *  If the timeou value is negative, the read operation will not use a timeout.
 *
 *  读取socket上的一个有效字节。
 *  如果timeout为负时，read操作不包含超时。
 */
- (void)readDataWithTimeout:(NSTimeInterval)timeout tag:(long)tag;

/**
 *  Read the first available bytes that become available on the socket.
 *  The bytes will be appended to the given byte buffer starting at the given offset.
 *
 *  If the timeout value is negative, the read operation will not use a timeout.
 *  If the buffer if nil, the socket will create a buffer for you.
 *  
 *  If the bufferOffset is greater than the length of the given buffer,
 *  the method will do noting, and the delegate will not be called.
 *  
 *  If you pass a buffer, you must not alter it in any way while the socket is using it.
 *  After completion, the data return in socket:didReadData:withTag: will be a subset of the given buffer.
 *  That is, it will reference the btyes that were appended to the given buffer via the method [NSData dataWithBytesNoCopy:length:freeWhenDone:NO];
 *
 *  读取有效字节。
 *  添加到给出buffer的偏移量的后面。
 *  如果传入的buffer为空，则socket会创建一个buffer。
 *  如果offset超出了buffur的长度，则这个方法什么也不执行，代理方法也不会被调用。
 *  在socket使用期间，你不可以修改buffer。
 *  每一个操作完成后，会通过socket:didReadData:withTag:返回对应的数据。这些数据就是通过[NSData dataWithBytesNoCopy:length:freeWhenDone:NO]方法追加到buffer上的。
 *
 */
- (void)readDataWithTimeout:(NSTimeInterval)timeout
                     buffer:(nullable NSMutableData *)buffer
               bufferOffset:(NSUInteger)bufferOffset
                        tag:(long)tag;

/**
 *  The given buffer will automatically be increased in size if need.
 *  A maximum of length bytes will be read.
 *  If maxLength is zero, no length restriction is enforced.
 *
 *  传入的buffer会自动扩容，最大值是maxLength。如果maxLength = 0 ，则表示没有限制。
 */
- (void)readDataWithtimeout:(NSTimeInterval)timeout
                     buffer:(nullable NSMutableData *)buffer
               bufferOffset:(NSUInteger)bufferOffset
                  maxLength:(NSUInteger)maxLength
                        tag:(long)tag;

/**
 *  Read the given number of bytes.
 *  If the length is 0, this method does nothing and the delegate is not called.
 *
 *  读取制定长度的字节
 *  如果length参数为0，则这个方法什么也不做，代理也不会被调用。
 *
 */
- (void)readDataToLength:(NSUInteger)length withTimeout:(NSTimeInterval)timeout tag:(long)tag;

/**
 * @see readDataWithTimeout:(NSTimeInterval)timeout buffer:(nullable NSMutableData *)buffer bufferOffset:(NSUInteger)bufferOffset tag:(long)tag
 */
- (void)readToLength:(NSUInteger)length
         withTimeout:(NSTimeInterval)timeout
              buffer:(nullable NSMutableData *)buffer
        bufferOffset:(NSUInteger)bufferOffset
                 tag:(long)tag;


@end
