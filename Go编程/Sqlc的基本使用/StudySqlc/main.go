package main

import (
	db "StudySqlc/db/gen"
	"context"
	"database/sql"
	"fmt"
	_ "github.com/go-sql-driver/mysql"
	"time"
)

func main() {
	// 连接 MySQL 数据库
	dsn := "root:root@tcp(192.168.1.103:3306)/gormdb?parseTime=true"
	dbConn, err := sql.Open("mysql", dsn)
	if err != nil {
		fmt.Printf("Failed to open DB: %v\n", err)
		return
	}
	defer dbConn.Close()

	// 确认数据库连接可用
	if err := dbConn.Ping(); err != nil {
		fmt.Printf("Failed to ping DB: %v\n", err)
		return
	}

	// 创建 Queries 实例
	queries := db.New(dbConn)

	ctx, cancel := context.WithTimeout(context.Background(), 5*time.Second)
	defer cancel()

	// 创建用户，检查错误
	res, err := queries.CreateUser(ctx, db.CreateUserParams{
		Name:  "admin_xy",
		Email: "admin_xy@163.com",
	})
	if err != nil {
		fmt.Printf("CreateUser failed: %v\n", err)
		return
	}

	// 取出 lastInsertID，检查错误
	lastInsertID, err := res.LastInsertId()
	if err != nil {
		fmt.Printf("LastInsertId failed: %v\n", err)
		return
	}
	fmt.Printf("Created user ID: %d\n", lastInsertID)

	// 查询单个用户，检查错误
	user, err := queries.GetUserByID(ctx, int32(lastInsertID))
	if err != nil {
		fmt.Printf("GetUserByID failed: %v\n", err)
		return
	}
	fmt.Printf("User fetched: %#v\n", user)

	// 查询所有用户，检查错误
	users, err := queries.ListUsers(ctx)
	if err != nil {
		fmt.Printf("ListUsers failed: %v\n", err)
		return
	}

	fmt.Println("All users:")
	for _, u := range users {
		fmt.Printf("- %#v\n", u)
	}
}
