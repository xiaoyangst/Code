package main

import (
	"StudyGo/stream_grpc_test/proto"
	"google.golang.org/grpc"
	"io"
	"log"
	"net"
	"strconv"
)

const PORT = ":50052"

type server struct {
	proto.UnimplementedGreeterServer
}

// 实现服务端流方法,消息是单向的,客户端发送消息,服务端返回消息流
// 服务端接受到消息之后，可以返回多个消息，直到消息发送完毕
func (s *server) GetStream(rep *proto.StreamReqData, res proto.Greeter_GetStreamServer) error {
	i := 0
	for {
		i++
		_ = res.Send(&proto.StreamResData{
			Data: "res: " + rep.Data + strconv.Itoa(i),
		})
		if i > 10 {
			break
		}
	}
	return nil
}

// 实现客户端流方法,消息是单向的,客户端发送消息流,服务端返回消息
// 客户端可以发送多个消息，直到消息发送完毕
func (s *server) PutStream(cliStr proto.Greeter_PutStreamServer) error {
	for {
		a, err := cliStr.Recv()
		if err == io.EOF {
			log.Println("client stream finished")
			return nil
		}
		if err != nil {
			log.Printf("failed to recv: %v", err)
			return err
		}
		log.Printf("recv client stream data: %v", a.Data)
	}
}

// 实现双向流方法,消息是双向的,客户端发送消息流,服务端返回消息流
// 客户端和服务端可以发送多个消息，直到消息发送完毕
// 客户端和服务端可以同时发送和接收消息
func (s *server) AllStream(allStr proto.Greeter_AllStreamServer) error {
	for {
		a, err := allStr.Recv()
		if err == io.EOF {
			log.Println("client stream finished")
			return nil
		}
		if err != nil {
			log.Printf("failed to recv: %v", err)
			return err
		}
		log.Printf("recv client stream data: %v", a.Data)
		_ = allStr.Send(&proto.StreamResData{
			Data: "server stream res: " + a.Data,
		})
	}
}

func main() {
	// 监听
	lis, err := net.Listen("tcp", PORT)
	if err != nil {
		log.Fatalf("failed to listen: %v", err)
		return
	}

	log.Printf("server listening at %v", lis.Addr())

	// 注册服务
	s := grpc.NewServer()
	proto.RegisterGreeterServer(s, &server{})

	// 启动服务
	if err := s.Serve(lis); err != nil {
		log.Fatalf("failed to serve: %v", err)
		return
	}

}
