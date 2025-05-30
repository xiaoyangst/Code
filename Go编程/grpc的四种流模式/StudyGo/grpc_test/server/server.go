package main

import (
	hello "StudyGo/grpc_test/proto"
	"context"
	"google.golang.org/grpc"
	"log"
	"net"
)

// Server Server结构体，实现Greeter服务接口
type Server struct {
	hello.UnimplementedGreeterServer // 内嵌未实现的基类
}

// SayHello SayHello方法实现，处理客户端请求
func (s *Server) SayHello(ctx context.Context, req *hello.HelloRequest) (*hello.HelloReply, error) {
	// 返回包含问候语的响应
	return &hello.HelloReply{Message: "Hello " + req.Name}, nil
}

func main() {
	// 创建新的gRPC服务器实例
	grpcServer := grpc.NewServer()

	// 注册 Greeter 服务实现，即发布本地服务为远程服务
	hello.RegisterGreeterServer(grpcServer, &Server{})

	// 监听 50051 端口的 TCP 连接
	lis, err := net.Listen("tcp", ":50051")
	if err != nil {
		log.Fatalf("failed to listen: %v", err) // 监听失败时记录错误并退出
	}

	// 打印服务启动日志
	log.Println("gRPC server listening on :50051")

	// 启动gRPC服务器，监听传入的连接
	err = grpcServer.Serve(lis)
	if err != nil {
		return
	}
}
