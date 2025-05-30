package main

import (
	"StudyGo/grpc_test/proto"
	"context"
	"fmt"
	"google.golang.org/grpc"
)

func main() {
	// 建立与 gRPC 服务器的连接，使用不安全连接(用于测试)
	conn, er := grpc.Dial("127.0.0.1:50051", grpc.WithInsecure())
	if er != nil {
		panic(er) // 如果连接失败，直接panic
	}
	defer func(conn *grpc.ClientConn) {
		err := conn.Close()
		if err != nil {
			panic(err)
		}
	}(conn) // 确保在函数退出时关闭连接

	// 创建 Greeter 服务的客户端
	c := proto.NewGreeterClient(conn)

	// 调用 SayHello 方法，传入 context 和请求参数
	r, err := c.SayHello(context.Background(), &proto.HelloRequest{Name: "world"})
	if err != nil {
		panic(err) // 如果调用失败，直接panic
	}
	fmt.Println(r.Message) // 打印服务器返回的消息
}
