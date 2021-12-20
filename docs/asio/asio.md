# ASIO - net.hpp

- asio/ts/netfwd.hpp
- asio/ts/executor.hpp
- asio/ts/io_context.hpp
- asio/ts/timer.hpp
- asio/ts/buffer.hpp
- asio/ts/socket.hpp
- asio/ts/internet.hpp

## netfwd - netfwd.hpp



## buffer - buffer.hpp

### asio/buffer.hpp

```c++
class mutable_buffer;
class const_buffer;
mutable_buffer buffer(...);
class dynamic_string_buffer;
dynamic_string_buffer dynamic_buffer();
```



### asio/completion_condition.hpp

```c++
boost::array<char, 128> raw_buf;
boost:system::error_code ec;
size_t n = boost::asio::read(
    sock,
    boost::asio::buffer(raw_buf),
	boost::asio::tranfer_all(),
	ec);
transfer_all_t transfer_all();
transfer_at_least_t transfer_at_least(size);
transfer_exactly_t transfer_exactly(size);
```



### asio/read.hpp

```c++
boost::array<char, 128> raw_buf;
boost:system::error_code ec;
size_t n = boost::asio::read(
    sock,
    boost::asio::buffer(raw_buf),
	boost::asio::tranfer_all(),
	ec
);
auto res = async_read(
	sock,
    boost::asio::buffer(raw_buf),
    ReadHandler
);
```



### asio/write.hpp

```c++
boost::array<char, 128> raw_buf;
boost:system::error_code ec;
size_t n = boost::asio::write(
    sock,
    boost::asio::buffer(raw_buf),
	boost::asio::tranfer_all(),
	ec
);
auto res = asynnc_write(
	sock,
    boost::asio::buffer(raw_buf),
    boost::asio::transfer_all(),
	WriteHandler
);
```



### asio/read_until.hpp

## executor - executor.hpp

asio/async_result.hpp

asio/associated_allocator.hpp

asio/execution_context.hpp

```c++
class io_context: public execution_context;
class execution_context { // base class
public:
    class id;
    class service;
    
    service_registry* service_registry_;
};

class service_registry {
	mutex mutex_;
    execution_context& owner_;
    execution_context::service* first_service_;
};

class execution_context::service { /* abstract base class*/
private:
    struct {
        typeinfo type_info_;
        execution_context::id* id_;
    } key_;
    execution_context* owner_;
    service* next;
};
```



asio/is_executor.hpp

asio/associated_executor.hpp

asio/bind_executor.hpp

asio/executor_work_guard.hpp

asio/system_executor.hpp

asio/executor.hpp

asio/any_io_executor.hpp

asio/dispatch.hpp

asio/post.hpp

asio/defer.hpp

asio/strand.hpp

asio/packaged_task.hpp

asio/use_future.hpp

## internet - internet.hpp

asio/ip/address.hpp

asio/ip/address_v4.hpp

asio/ip/address_v4_iterator.hpp

asio/ip/address_v4_range.hpp

asio/ip/address_v6.hpp

asio/ip/address_v6_iterator.hpp

asio/ip/address_v6_range.hpp

asio/ip/bad_address_cast.hpp

asio/ip/basic_endpoint.hpp

asio/ip/basic_resolver_query.hpp

asio/ip/basic_resolver_entry.hpp

asio/ip/basic_resolver_iterator.hpp

asio/ip/basic_resolver.hpp

asio/ip/host_name.hpp

asio/ip/network_v4.hpp

asio/ip/network_v6.hpp

asio/ip/tcp.hpp

asio/ip/udp.hpp

asio/ip/v6_only.hpp

asio/ip/unicast.hpp

asio/ip/multicast.hpp

## io_context - io_context.hpp

asio/io_context.hpp

## socket - socket.hpp

asio/socket_base.hpp

asio/basic_socket.hpp

asio/basic_datagram_socket.hpp

asio/basic_stream_socket.hpp

asio/basic_socket_acceptor.hpp

asio/basic_socket_streambuf.hpp

asio/basic_socket_iostream.hpp

asio/connect.hpp

## timer - timer.hpp

asio/detail/chrono.hpp



asio/wait_traits.hpp

asio/basic_waitable_timer.hpp

asio/system_timer.hpp

asio/steady_timer.hpp

asio/high_resolution_timer.hpp