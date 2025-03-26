#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <mariadb/conncpp.hpp>
using namespace std;
class database
{
    private:

    public:
        void addMember(std::unique_ptr<sql::Connection> &conn, std::string id, std::string pw, std::string name, std::string phone, std::string addr)  
        {
			try {
				// PreparedStatement 객체 생성
				std::unique_ptr<sql::PreparedStatement> stmnt(conn->prepareStatement("insert into Member values (?, ?, ?, ?, ?, default,'1','0','0',NULL)"));
				// 객체에 값을 전달
				stmnt->setString(1, id);
				stmnt->setString(2, pw);
				stmnt->setString(3, name);
				stmnt->setString(4, phone);
				stmnt->setString(5, addr);
				// 객체의 내부 함수를 이용하여 쿼리를 실행
				stmnt->executeUpdate();
			// 실패시 오류 메세지 반환
			} catch(sql::SQLException& e){
			std::cerr << "Error inserting new Member: " << e.what() << std::endl;
			}
		}
        int checkLogin(std::unique_ptr<sql::Connection> &conn, std::string id, std::string pw) 
        {
            try {
                std::unique_ptr<sql::PreparedStatement> stmnt(conn->prepareStatement("SELECT M_id , M_pw FROM Member WHERE M_id = ? and M_pw =?"));
                stmnt->setString(1, id);
                stmnt->setString(2, pw);

                std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery());
                if (res->next()) {
                    return 1; //맞으면 로그인 성공
                } else {
                    return 2; // 중복되지 않은 경우 빈 문자열 반환
                }

            } catch(sql::SQLException& e) {
                std::cerr << "Error selecting tasks: " << e.what() << std::endl;
                return 3;
            }
        }
		int checkLoginId(std::unique_ptr<sql::Connection> &conn, std::string id) 
        {
			try {
				std::unique_ptr<sql::PreparedStatement> stmnt(conn->prepareStatement("SELECT M_id FROM Member WHERE M_id = ?"));
				stmnt->setString(1, id);

				std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery());
				if (res->next()) {
					return 1; //맞으면 로그인 성공
				} else {
					return 2; // 중복되지 않은 경우 빈 문자열 반환
				}

			} catch(sql::SQLException& e) {
				std::cerr << "Error selecting tasks: " << e.what() << std::endl;
				return 3;
			}
		}
		int checkLoginPw(std::unique_ptr<sql::Connection> &conn, std::string pw) {
			try {
				std::unique_ptr<sql::PreparedStatement> stmnt(conn->prepareStatement("SELECT  M_pw FROM Member WHERE  M_pw = ?"));
				stmnt->setString(1, pw);

				std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery());
				if (res->next()) {
					return 1; //맞으면 로그인 성공
				} else {
					return 2; // 중복되지 않은 경우 빈 문자열 반환
				}

			} catch(sql::SQLException& e) {
				std::cerr << "Error selecting tasks: " << e.what() << std::endl;
				return 3;
			}
		}
        int rating(std::unique_ptr<sql::Connection> &conn,std::string id) 
        {
            try {
                std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
                std::string str = "select M_rating from Member where M_id = '"+id+"'";
                std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery(str));
                // 반복문을 통해서 내부의 값을 반환
                if(res->next()) {
                    return res->getInt(1);
                }
                else
                    return 3;
                
            // 실패시 오류 메세지 반환
            } catch(sql::SQLException& e){
                std::cerr << "Error selecting tasks: " << e.what() << std::endl;
                return 3;
            }
        }
        int abnormal_return(std::unique_ptr<sql::Connection> &conn,std::string id) 
        {
            try {
                std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
                std::string str = "select M_abnormal_return from Member where M_id = '"+id+"'";
                std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery(str));
                // 반복문을 통해서 내부의 값을 반환
                if(res->next()) {
                    return res->getInt(1);
                }
                else
                    return -1;
                
            // 실패시 오류 메세지 반환
            } catch(sql::SQLException& e){
                std::cerr << "Error selecting tasks: " << e.what() << std::endl;
                return -1;
            }
        }
        int Subscription_period(std::unique_ptr<sql::Connection> &conn,std::string id) 
        {
            try {
                std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
                std::string str = "select datediff(curdate(),(select M_join_date from Member where M_id = '"+id+"'))";
                std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery(str));
                // 반복문을 통해서 내부의 값을 반환
                if(res->next()) {
                    return res->getInt(1);
                }
                else
                    return -1;
                
            // 실패시 오류 메세지 반환
            } catch(sql::SQLException& e){
                std::cerr << "Error selecting tasks: " << e.what() << std::endl;
                return -1;
            }
        }
        int Date_comparison(std::unique_ptr<sql::Connection> &conn,std::string id,std::string book) 
        {
            try {
                std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
                std::string str = "select datediff(curdate(),(select date from borrow where book_num = '"+book+"' and user_id = '"+id+"'))";
                std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery(str));
                // 반복문을 통해서 내부의 값을 반환
                if(res->next()) {
                    return res->getInt(1);
                }
                else
                    return -1;
                
            // 실패시 오류 메세지 반환
            } catch(sql::SQLException& e){
                std::cerr << "Error selecting tasks: " << e.what() << std::endl;
                return -1;
            }
        }
        bool changemember(std::unique_ptr<sql::Connection> &conn,std::string id) 
        {
            try {
                std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
                std::string str = "select datediff(curdate(),(select black_date from Member where M_id = '"+id+"'))";
                std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery(str));
                // 반복문을 통해서 내부의 값을 반환
                if(res->next()) {
                    if(res->getInt(1) >= 30)
                    {
                        return 1;
                    }
                    else
                        return 0;
                }
                else
                    return 0;
                
            // 실패시 오류 메세지 반환
            } catch(sql::SQLException& e){
                std::cerr << "Error selecting tasks: " << e.what() << std::endl;
                return 0;
            }
        }
        int Normal_return(std::unique_ptr<sql::Connection> &conn,std::string id) 
        {
            try {
                std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
                std::string str = "select M_normal_return from Member where M_id = '"+id+"'";
                std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery(str));
                // 반복문을 통해서 내부의 값을 반환
                if(res->next()) {
                    return res->getInt(1);
                }
                else
                    return -1;
                

            // 실패시 오류 메세지 반환
            } catch(sql::SQLException& e){
                std::cerr << "Error selecting tasks: " << e.what() << std::endl;
                return -1;
            }
        }
        void Excellent_member(std::unique_ptr<sql::Connection> &conn,std::string id) 
        {
            try {
                std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
                std::string str = "update Member set M_rating = '2' where M_id = '"+id+"'";
                std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery(str));
                // 반복문을 통해서 내부의 값을 반환
                while (res->next()) {
                    
                }
            
            // 실패시 오류 메세지 반환
            } catch(sql::SQLException& e){
                std::cerr << "Error selecting tasks: " << e.what() << std::endl;
            }
        }
        void Black_member(std::unique_ptr<sql::Connection> &conn,std::string id) 
        {
            try {
                std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
                std::string str = "update Member set M_rating = '0' where M_id = '"+id+"'";
                std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery(str));
                // 반복문을 통해서 내부의 값을 반환
                while (res->next()) {
                    
                }
            
            // 실패시 오류 메세지 반환
            } catch(sql::SQLException& e){
                std::cerr << "Error selecting tasks: " << e.what() << std::endl;
            }
            updateabnormalzero(conn,id);
        }
        void normal_member(std::unique_ptr<sql::Connection> &conn,std::string id) 
        {
            try {
                std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
                std::string str = "update Member set M_rating = '1' where M_id = '"+id+"'";
                std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery(str));
                // 반복문을 통해서 내부의 값을 반환
                while (res->next()) {
                    
                }
            
            // 실패시 오류 메세지 반환
            } catch(sql::SQLException& e){
                std::cerr << "Error selecting tasks: " << e.what() << std::endl;
            }
        }
        void updatenormal(std::unique_ptr<sql::Connection> &conn,std::string id) 
        {
            try {
                std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
                std::string str = "update Member set M_normal_return = M_normal_return +1 where M_id = '"+id+"'";
                std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery(str));
                // 반복문을 통해서 내부의 값을 반환
                while (res->next()) {
                    
                }
            
            // 실패시 오류 메세지 반환
            } catch(sql::SQLException& e){
                std::cerr << "Error selecting tasks: " << e.what() << std::endl;
            }
        }
        void updateabnormal(std::unique_ptr<sql::Connection> &conn,std::string id) 
        {
            try {
                std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
                std::string str = "update Member set M_abnormal_return = M_abnoraml_return + 1 where M_id = '"+id+"'";
                std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery(str));
                // 반복문을 통해서 내부의 값을 반환
                while (res->next()) {
                    
                }
            
            // 실패시 오류 메세지 반환
            } catch(sql::SQLException& e){
                std::cerr << "Error selecting tasks: " << e.what() << std::endl;
            }
        }
        void updateabnormalzero(std::unique_ptr<sql::Connection> &conn,std::string id) 
        {
            try {
                std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
                std::string str = "update Member set M_abnormal_return = 0 where M_id = '"+id+"'";
                std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery(str));
                // 반복문을 통해서 내부의 값을 반환
                while (res->next()) {
                    
                }
            
            // 실패시 오류 메세지 반환
            } catch(sql::SQLException& e){
                std::cerr << "Error selecting tasks: " << e.what() << std::endl;
            }
        }
        void black_date(std::unique_ptr<sql::Connection> &conn,std::string id) 
        {
            try {
                std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
                std::string str = "update Member set black_date = default where M_id = '"+id+"'";
                std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery(str));
                // 반복문을 통해서 내부의 값을 반환
                while (res->next()) {
                    
                }
            
            // 실패시 오류 메세지 반환
            } catch(sql::SQLException& e){
                std::cerr << "Error selecting tasks: " << e.what() << std::endl;
            }
        }
        void nameselect(std::unique_ptr<sql::Connection> &conn,string userinput,string book[],int  clnt_sock,int number) 
        {
            try {
                std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
                std::string str = "SELECT * FROM KING where name like '%" + userinput +"%'limit 10 offset "+ std::to_string(number);
                std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery(str));
                // 반복문을 통해서 내부의 값을 반환
                while (res->next()) {
                    for(int i = 0 ; i < 10 ; i++)
                    {
                        book[i] = res->getString(i+1);
                        int len = book[i].size();
                        write(clnt_sock,&len,sizeof(len));
                        write(clnt_sock,book[i].c_str(),len);
                    }
                }
                

            // 실패시 오류 메세지 반환
            } catch(sql::SQLException& e){
                std::cerr << "Error selecting tasks: " << e.what() << std::endl;
            }
        }
        void Authorselect(std::unique_ptr<sql::Connection> &conn,string userinput,string book[],int  clnt_sock,int number) 
        {
            try {
                std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
                std::string str = "SELECT * FROM KING where author like '%" + userinput +"%'limit 10 offset "+ std::to_string(number);
                std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery(str));
                // 반복문을 통해서 내부의 값을 반환
                while (res->next()) {
                    for(int i = 0 ; i < 10 ; i++)
                    {
                        book[i] = res->getString(i+1);
                        int len = book[i].size();
                        write(clnt_sock,&len,sizeof(len));
                        write(clnt_sock,book[i].c_str(),len);
                    }
                }
                

            // 실패시 오류 메세지 반환
            } catch(sql::SQLException& e){
                std::cerr << "Error selecting tasks: " << e.what() << std::endl;
            }
        }
        void Call_numberselect(std::unique_ptr<sql::Connection> &conn,string userinput,string book[],int clnt_sock,int number) 
        {
            try {
                std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
                std::string str = "SELECT * FROM KING where find like '" + userinput + "%'limit 10 offset "+ std::to_string(number);

                std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery(str));
                // 반복문을 통해서 내부의 값을 반환
                while (res->next()) {
                    for(int i = 0 ; i < 10 ; i++)
                    {
                        book[i] = res->getString(i+1);
                        int len = book[i].size();
                        write(clnt_sock,&len,sizeof(len));
                        write(clnt_sock,book[i].c_str(),len);
                    }
                }
            // 실패시 오류 메세지 반환
            } catch(sql::SQLException& e){
                std::cerr << "Error selecting tasks: " << e.what() << std::endl;
            }
        }
        void borrowBook(std::unique_ptr<sql::Connection> &conn, std::string id, std::string book) {
            try {
                std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
                std::string str = "insert into borrow value ('"+book+"','"+id+"',default)";
                std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery(str));

            } catch(sql::SQLException& e) {
                std::cerr << "Error selecting tasks: " << e.what() << std::endl;
            }
        }
        bool borrowcheck(std::unique_ptr<sql::Connection> &conn, std::string book) {
            try {
                std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
                std::string str = "select book_num from borrow where book_num = '"+book+"'";
                std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery(str));
                if(res->next()) {
                    return 1;
                }
                else
                    return 0;
            } catch(sql::SQLException& e) {
                std::cerr << "Error selecting tasks: " << e.what() << std::endl;
                return 0;
            }
        }
        int returnBook(std::unique_ptr<sql::Connection> &conn, std::string id, std::string book) 
        {
            try {
                std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
                std::string str = "select * from borrow where book_num = '"+book+"' and user_id = '"+id+"'";
                std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery(str));
            if(res->next()) 
            {
                std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
                std::string str = "delete from borrow where book_num = '"+book+"' and user_id = '"+id+"'";
                std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery(str));
                return 1;
            }
            else 
                return 0;

            } catch(sql::SQLException& e) {
                std::cerr << "Error executing tasks: " << e.what() << std::endl;
                return 0;
            }
        }
};
class server
{
    private:
        int serv_sock;
        int clnt_sock;
        int option;
        socklen_t optlen;
        struct sockaddr_in serv_adr;
        struct sockaddr_in clnt_addr;
        socklen_t clnt_addr_size;
        char message[1024];
        char number;
        char id[1024];
        string book[10] = {"연변"," 소장도서관"," 자료실명"," 등록번호"," 도서명"," 저자"," 출판사"," 출판연도"," 청구기호"," 데이터기준일자"};
    public:
        void input()
        {
            for(int i = 0 ; i < 10 ; i++)
            {
                int len = book[i].size();
                write(clnt_sock,&len,sizeof(len));
                write(clnt_sock,book[i].c_str(),len);
            }  
        }
        void login(std::unique_ptr<sql::Connection> &conn,database date_class)
        {
            while(1)
            {
                number = '0';
                int str_len=read(clnt_sock,(void *)&number,1);
                if(str_len==-1)
                    error("read() error!");
                if(number == '9')
                {
                    char pw[1024];
                    while(1)
                    {
                        str_len=read(clnt_sock,id, 1024);
                        str_len=read(clnt_sock,pw, 1024);
                        if(str_len==-1)
                            error("read() error!");
                        int loginid = date_class.checkLoginId(conn, id);
                        int loginpw = date_class.checkLoginPw(conn, pw);
                        int login = date_class.checkLogin(conn,id, pw);
                        if (loginid == 1 && loginpw == 1 && login == 1) 
                        {
                            number = '1';
                            write(clnt_sock,(void *)&number, 1);
                            break;
                        } 
                        else 
                        {
                            number = '2';
                            write(clnt_sock,(void *)&number, 1);
                        }
                    }
                }
                else if(number == '8')
                {
                    char pw[1024];
                    char name[1024];
                    char phone[1024];
                    char addr[1024];
                    while(1)
                    {
                        str_len=read(clnt_sock,id, 1024);
                        str_len=read(clnt_sock,pw, 1024);
                        str_len=read(clnt_sock,name, 1024);
                        str_len=read(clnt_sock,phone, 1024);
                        str_len=read(clnt_sock,addr, 1024);
                        if(str_len==-1)
                            error("read() error!");
                        int loginid = date_class.checkLoginId(conn, id);
                        if(loginid == 1)
                        {
                            number = '1';
                            write(clnt_sock,(void *)&number, 1);
                        }
                        else
                        {
                            number = '9';
                            write(clnt_sock,(void *)&number, 1);
                            date_class.addMember(conn,id,pw,name,phone,addr);
                        }
                    }
                }
                // else if(number == '1')
                // {
                //     for(int i = 0 ; i < 10 ; i++)
                //     {
                //         int len = book[i].size();
                //         write(clnt_sock,&len,sizeof(len));
                //         write(clnt_sock,book[i].c_str(),len);
                //     }  
                // }
                else if(number == '1')
                {
                    read(clnt_sock,(void *)&number,1);
                    if(number == '1')
                    {
                        read(clnt_sock,message, 1024);
                        while(1)
                        {
                            read(clnt_sock,(void *)&number,1);
                            if(number == -1)
                            {
                                break;
                            }
                            date_class.Authorselect(conn,message,book,clnt_sock,number);
                        }
                    }
                    else if(number == '2')
                    {
                        read(clnt_sock,message, 1024);
                        while(1)
                        {
                            read(clnt_sock,(void *)&number,1);
                            if(number == -1)
                            {
                                break;
                            }
                            date_class.Call_numberselect(conn,message,book,clnt_sock,number);
                        }
                    }
                    else if(number == '3')
                    {
                        read(clnt_sock,message, 1024);
                        while(1)
                        {
                            read(clnt_sock,(void *)&number,1);
                            if(number == -1)
                            {
                                break;
                            }
                            date_class.nameselect(conn,message,book,clnt_sock,number);
                        }
                    }
                }
                else if(number == '2')
                {
                    int num = date_class.rating(conn,id);
                    if(num == 0)
                    {
                        number = '1';
                        write(clnt_sock,(void *)&number, 1);
                    }
                    else
                    {
                        number = '0';
                        write(clnt_sock,(void *)&number, 1);
                        read(clnt_sock,message, 1024);
                        if(date_class.borrowcheck(conn,message))
                        {
                            number ='1';
                            write(clnt_sock,(void *)&number, 1);
                        }
                        else
                        {
                            number ='0';
                            write(clnt_sock,(void *)&number, 1);
                            read(clnt_sock,(void *)&number,1);
                            if(number == '1')
                            {
                                date_class.borrowBook(conn,id,message);
                            }
                        }
                    }
                }
                else if(number == '3')
                {
                    read(clnt_sock,message, 1024);
                    if(date_class.borrowcheck(conn,message))
                    {
                        number ='1';
                        write(clnt_sock,(void *)&number, 1);
                        read(clnt_sock,(void *)&number,1);
                        if(number == '1')
                        {
                            int date =date_class.Date_comparison(conn,id,message);
                            int bools = date_class.returnBook(conn,id,message);
                            if(bools == 1)
                            {
                                number ='1';
                                write(clnt_sock,(void *)&number, 1);
                                int num = date_class.rating(conn,id);
                                if(num <= 1)
                                {
                                    if(date <= 4)
                                    {
                                        date_class.updatenormal(conn,id);
                                    }
                                    else if (date > 14)
                                    {
                                        date_class.Black_member(conn,id);
                                    }
                                    else if (date > 4)
                                    {
                                        date_class.updateabnormal(conn,id);
                                    }
                                }
                                else if(num == 2)
                                {
                                    if(date <= 7)
                                    {
                                        date_class.updatenormal(conn,id);
                                    }
                                    else if (date > 14)
                                    {
                                        date_class.Black_member(conn,id);
                                    }
                                    else if (date > 7)
                                    {
                                        date_class.updateabnormal(conn,id);
                                    }

                                }


                            }
                            else if(bools == 0)
                            {
                                number ='0';
                                write(clnt_sock,(void *)&number, 1);
                            }
                        }
                        else
                            number ='0';
                            write(clnt_sock,(void *)&number, 1);
                    }
                    else
                    {
                        number ='0';
                        write(clnt_sock,(void *)&number, 1);
                    }
                }
                else if(number == '0')
                {
                    sleep(3);
                }
                Membership_renewal(conn,date_class);
            }
        }
        void Membership_renewal(std::unique_ptr<sql::Connection> &conn,database date_class)
        {
            int Subscription = date_class.Subscription_period(conn,id);
            int normal_return = date_class.Normal_return(conn,id);
            if(Subscription >= 180 && normal_return >= 10)
            {
                date_class.Excellent_member(conn,id);
            }
            int abnormal_return = date_class.abnormal_return(conn,id);
            if(abnormal_return >= 3)
            {
                date_class.Black_member(conn,id);
                date_class.black_date(conn,id);
            }
            if(date_class.changemember(conn,id))
            {
                date_class.normal_member(conn,id);
            }
        }
        void create_socket(int * argc,char * argv[])
        {
            if(*argc != 2)
            {
                cout<<"Usage : "<<argv[0]<<"<port>"<<endl;
                exit(1);
            }

            serv_sock = socket(PF_INET,SOCK_STREAM,0);
            if(serv_sock == -1)
            {
                error("socket() error");
            }

            optlen = sizeof(option);
            option = 1;
            setsockopt(serv_sock, SOL_SOCKET,SO_REUSEADDR,(void*)&option,optlen);

            memset(&serv_adr, 0, sizeof(serv_adr));
            serv_adr.sin_family=AF_INET;
            serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
            serv_adr.sin_port=htons(atoi(argv[1]));

            if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
                error("bind() error");
            if(listen(serv_sock, 2)==-1)
                error("listen() error");

            clnt_addr_size=sizeof(clnt_addr);  
            clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_size);
            if(clnt_sock==-1)
            {
                error("accept() error");  
            }

        }
        void error(const char * errormessage)
        {
            fputs(errormessage, stderr);
            fputc('\n', stderr);
            exit(1);
        }
        void socket_close()
        {
            close(clnt_sock);	
	        close(serv_sock);
        }
};
int main(int argc, char *argv[])
{
    sql::Driver* driver = sql::mariadb::get_driver_instance();
    sql::SQLString url("jdbc:mariadb://localhost:3306/KINGSEO");
    sql::Properties properties({{"user", "PMJ"}, {"password", "1234"}});
    std::unique_ptr<sql::Connection> conn(driver->connect(url, properties));
    server server_class;
    database date_class;
    server_class.create_socket(&argc,argv);
    server_class.login(conn,date_class);
    server_class.socket_close();
    return 0;
}