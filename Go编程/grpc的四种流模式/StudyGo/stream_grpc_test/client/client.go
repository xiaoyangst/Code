package main

import (
	"context"
	"google.golang.org/grpc"
	"log"
	"strconv"
)
import "StudyGo/stream_grpc_test/proto"

func main() {
	// 连接
	conn, err := grpc.Dial("localhost:50052", grpc.WithInsecure())
	if err != nil {
		panic(err)
	}
	defer conn.Close()

	// 创建客户端
	client := proto.NewGreeterClient(conn)

	// ========== 服务端流 ========== //
	log.Println(">>> start server stream:")
	// 调用服务端流方法
	getStream, _ := client.GetStream(context.Background(), &proto.StreamReqData{
		Data: "client stream req", // 客户端把消息发送给服务端
	})
	for {
		resp, err := getStream.Recv()
		if err != nil {
			log.Printf("GetStream finished: %v", err)
			break
		}
		log.Printf("recv server stream data: %v", resp.Data)
	}

	// ========== 客户端流 ========== //
	log.Println(">>> start client stream:")
	// 调用客户端流方法
	putStream, _ := client.PutStream(context.Background())
	for i := 0; i < 10; i++ {
		err := putStream.Send(&proto.StreamReqData{
			Data: "client stream req " + strconv.Itoa(i),
		})
		if err != nil {
			log.Printf("send error: %v", err)
			break
		}
	}

	// ========== 双向流 ========== //
	log.Println(">>> start all stream:")
	// 调用双向流方法
	allStream, _ := client.AllStream(context.Background())
	for i := 0; i < 10; i++ {
		err := allStream.Send(&proto.StreamReqData{
			Data: "client stream req " + strconv.Itoa(i),
		})
		if err != nil {
			log.Printf("send error: %v", err)
			break
		}
		resp, err := allStream.Recv()
		if err != nil {
			log.Printf("recv error: %v", err)
			break
		}
		log.Printf("recv server stream data: %v", resp.Data)
	}

	_ = putStream.CloseSend()
}
