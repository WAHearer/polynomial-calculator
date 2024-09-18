#include<iostream>
#include<cmath>
#include<stack>
#include<queue>
#include<map>
#include"cpp-httplib-master/httplib.h"
using namespace std;
class List{
	public:
		float a,n;
		List *next;
		void insert(List *node){
			if(node->a==0)
				return;
			List *p=this;
			while(p){
				if(!(p->next)){
					p->next=node;
					node->next=nullptr;
					return;
				}
				if(node->n>p->next->n){
					node->next=p->next;
					p->next=node;
					return;
				}
				else if(node->n==p->next->n){
					p->next->a+=node->a;
					if(p->next->a==0)
						p->next=p->next->next;
					return;
				}
				p=p->next;
			}
		}
		List(){
			this->a=this->n=0;
			this->next=nullptr;
		}
		explicit List(string s){
			this->a=this->n=0;
			int len=s.length();
			float a=0,n=0;
			List *p=(List*)malloc(sizeof(List));
			List *head=(List*)malloc(sizeof(List));
			head->next=nullptr;
			bool xAppeared=false,pointAppeared=false;
			int sign=1,decimalCnt=0;
			for(int i=0;i<len;i++){
				if('0'<=s[i]&&s[i]<='9'){
					if(!xAppeared){
						if(!pointAppeared)
							a=a*10+s[i]-'0';
						else{
							a=a+(s[i]-'0')*1.0/pow(10,decimalCnt);
							decimalCnt++;
						}
					}
					else{
						if(!pointAppeared)
							n=n*10+s[i]-'0';
						else{
							n=n+(s[i]-'0')*1.0/pow(10,decimalCnt);
							decimalCnt++;
						}
					}
				}
				else if(s[i]=='.'){
					pointAppeared=true;
					decimalCnt=1;
				}
				else if(s[i]=='x'){
					if(i==0||s[i-1]=='+'||s[i-1]=='-')
						a=1;
					p->a=a*sign;
					sign=1;
					pointAppeared=false;
					xAppeared=true;
				}
				else if(s[i]=='+'||s[i]=='-'){
					if(i!=0&&s[i-1]!='^'){
						if(!xAppeared){
							p->a=a*sign;
							sign=1;
						}
						if(s[i-1]=='x')
							n=1;
						p->n=n*sign;
						head->insert(p);
						p=(List*)malloc(sizeof(List));
						a=n=xAppeared=pointAppeared=decimalCnt=0;
					}
					if(s[i]=='+')
						sign=1;
					else if(s[i]=='-')
						sign=-1;
				}
			}
			if(!xAppeared){
				p->a=a*sign;
				sign=1;
			}
			if(s[len-1]=='x')
				n=1;
			p->n=n*sign;
			head->insert(p);
			this->next=head->next;
		}
		 string print(){
			ostringstream oss;
			string s;
			if(!this->next){
				return "0";
			}
			List *p=this->next;
			bool isFirst=true;
			while(p){
				if(isFirst)
					isFirst=false;
				else if(p->a>0)
					oss<<'+';
				else if(p->a==0)
					continue;
				if(p->n==0){
					oss<<p->a;
					p=p->next;
					continue;
				}
				if(p->a!=1&&p->a!=-1)
					oss<<p->a;
				else if(p->a==-1)
					oss<<"-";
				oss<<"x";
				if(p->n!=1)
					oss<<"^"<<p->n;
				p=p->next;
			}
			s=oss.str();
			return s;
		}
		string printAsNumbers(){
			ostringstream oss;
			string s;
			if(!this->next){
				return "0";
			}
			List *p=this->next;
			int cnt=0;
			while(p){
				cnt++;
				oss<<p->a<<" "<<p->n<<" ";
				p=p->next;
			}
			s=to_string(cnt)+" "+oss.str();
			return s;
		}
		List operator+(const List &y)const{
			List *head=(List*)malloc(sizeof(List));
			head->next=nullptr;
			List *p=next;
			while(p){
				List *node=(List*)malloc(sizeof(List));
				node->a=p->a;
				node->n=p->n;
				head->insert(node);
				p=p->next;
			}
			p=y.next;
			while(p){
				List *node=(List*)malloc(sizeof(List));
				node->a=p->a;
				node->n=p->n;
				head->insert(node);
				p=p->next;
			}
			List result=List();
			result.next=head->next;
			return result;
		}
		List operator-(const List &y)const{
			List *head=(List*)malloc(sizeof(List));
			head->next=nullptr;
			List *p=next;
			while(p){
				List *node=(List*)malloc(sizeof(List));
				node->a=p->a;
				node->n=p->n;
				head->insert(node);
				p=p->next;
			}
			p=y.next;
			while(p){
				List *node=(List*)malloc(sizeof(List));
				node->a=-p->a;
				node->n=p->n;
				head->insert(node);
				p=p->next;
			}
			List result=List();
			result.next=head->next;
			return result;
		}
		List operator*(const List&y)const{
			List *head=(List*)malloc(sizeof(List));
			head->next=nullptr;
			List *p1=next,*p2=y.next;
			while(p1){
				while(p2){
					List *node=(List*)malloc(sizeof(List));
					node->a=p1->a*p2->a;
					node->n=p1->n+p2->n;
					head->insert(node);
					p2=p2->next;
				}
				p1=p1->next;
				p2=y.next;
			}
			List result=List();
			result.next=head->next;
			return result;
		}
		List derivation(){
			List *head=(List*)malloc(sizeof(List));
			head->next=nullptr;
			List *p=next;
			while(p) {
				List *node=(List*)malloc(sizeof(List));
				node->a=p->a*p->n;
				node->n=p->n-1;
				head->insert(node);
				p=p->next;
			}
			List result=List();
			result.next=head->next;
			return result;
		}
};
struct Op{
	List num;
	char op;
	bool flag;
};
bool check(string s){
	int len=s.length();
	for(int i=0;i<len;i++){
		if(!isdigit(s[i])&&s[i]!='.'&&s[i]!='x'&&s[i]!='+'&&s[i]!='-'&&s[i]!='*'&&s[i]!='\''&&s[i]!='^'&&s[i]!='('&&s[i]!=')')
			return false;
	}
	int cnt=0;
	for(int i=0;i<len;i++){
		if(s[i]=='(')
			cnt++;
		else if(s[i]==')')
			cnt--;
		if(cnt<0)
			return false;
	}
	if(cnt!=0)
		return false;
	if(s[0]=='.'||s[0]=='*'||s[0]=='\''||s[0]=='^'||s[0]==')')
		return false;
	bool pointAppeared=false;
	for(int i=1;i<len;i++){
		if(isdigit(s[i])){
			if(s[i-1]==')'||s[i-1]=='x'||s[i-1]=='\''||s[i+1]=='^')
				return false;
		}
		if(s[i]=='.'){
			if(pointAppeared)
				return false;
			pointAppeared=true;
			if(!isdigit(s[i-1])||!isdigit(s[i+1]))
				return false;
		}
		if(s[i]=='x'){
			pointAppeared=false;
			if(s[i-1]=='.'||s[i-1]=='x'||s[i-1]=='\''||s[i-1]=='^'||s[i+1]=='.'||s[i+1]=='x')
				return false;
		}
		if(s[i]=='+'||s[i]=='-'){
			pointAppeared=false;
			if(s[i-1]=='.'||s[i-1]=='+'||s[i-1]=='-'||s[i-1]=='*'||s[i+1]=='.'||s[i+1]=='+'||s[i+1]=='-'||s[i+1]=='\''||s[i+1]=='^'||s[i+1]==')')
				return false;
		}
		if(s[i]=='*'){
			pointAppeared=false;
			if(s[i-1]=='.'||s[i-1]=='+'||s[i-1]=='-'||s[i-1]=='*'||s[i-1]=='^'||s[i-1]=='('||s[i+1]=='.'||s[i+1]=='+'||s[i+1]=='-'||s[i+1]=='\''||s[i+1]=='^'||s[i+1]==')')
				return false;
		}
		if(s[i]=='\''){
			pointAppeared=false;
			if(!isdigit(s[i-1])&&s[i-1]!='x'&&s[i-1]!=')')
				return false;
		}
		if(s[i]=='^'){
			if(s[i-1]!='x'||(!isdigit(s[i+1])&&s[i+1]!='-'))
				return false;
		}
		if(s[i]=='('||s[i]==')'){
			pointAppeared=false;
		}
	}
	return true;
}
stack<Op>st;
queue<Op>q;
map<char,int>priority;
void make(string s){
	int len=s.length();
	Op temp;
	for(int i=0;i<len;i++){
		if(s[i]=='('){
			temp.flag=false;
			temp.op='(';
			st.push(temp);
		}
		else if(s[i]==')'){
			while(st.top().op!='('){
				q.push(st.top());
				st.pop();
			}
			st.pop();
		}
		else if(isdigit(s[i])||s[i]=='x'||(i>0&&s[i-1]=='('&&s[i]=='-')){
			string now;
			while(isdigit(s[i])||s[i]=='.'||s[i]=='x'||s[i]=='^'||(s[i-1]=='^'&&s[i]=='-')||(i>0&&s[i-1]=='('&&s[i]=='-')){
				now+=s[i];
				i++;
			}
			List list=List(now);
			temp.num=list;
			temp.flag=true;
			q.push(temp);
			i--;
		}
		else if(st.empty()||st.top().op=='('||priority[st.top().op]<priority[s[i]]){
			temp.flag=false;
			temp.op=s[i];
			st.push(temp);
		}
		else{
			q.push(st.top());
			st.pop();
			i--;
		}
	}
	while(!st.empty()){
		q.push(st.top());
		st.pop();
	}
}
List cal(){
	Op temp;
	temp.num=List();
	while(!q.empty()){
		temp=q.front();
		q.pop();
		if(temp.flag)
			st.push(temp);
		else{
			if(temp.op=='\''){
				List num=st.top().num;
				st.pop();
				Op res;
				res.num=List();
				res.num=num.derivation();
				res.flag=true;
				st.push(res);
			}
			else{
				List num1=st.top().num,num2;
				st.pop();
				if(st.empty())
					num2=List();
				else{
					num2=st.top().num;
					st.pop();
				}
				Op res;
				res.flag=true;
				res.num=List();
				if(temp.op=='+'){
					res.num=num1+num2;
				}
				if(temp.op=='-'){
					res.num=num2-num1;
				}
				if(temp.op=='*'){
					res.num=num1*num2;
				}
				st.push(res);
			}
		}
	}
	List result=st.top().num;
	while(!st.empty())
		st.pop();
	return result;
}
int main(){
	priority['\'']=3;
	priority['*']=2;
	priority['+']=priority['-']=1;
	httplib::Server server;
	server.set_default_headers({
		{ "Access-Control-Allow-Origin" ,"*" },
		{ "Access-Control-Allow-Methods","POST, GET, PUT, OPTIONS, DELETE"},
		{ "Access-Control-Max-Age","3600"},
		{ "Access-Control-Allow-Headers","*"},
		{ "Content-Type","text/plain"}
	});
	server.Post("/calc",[](const httplib::Request &req,httplib::Response &res){
		bool mode;
		string s=req.body;
		if(s[0]=='?'){
			mode=true;
			s.erase(0,1);
		}
		else
			mode=false;
		if(!check(s))
			res.set_content("invalid","text/plain");
		else{
			int len=s.length();
			for(int i=1;i<len;i++){
				if(s[i]=='('&&s[i-1]!='+'&&s[i-1]!='-'&&s[i-1]!='*'){
					s.insert(s.begin()+i,'*');
					len++;
					i++;
				}
				if(i>0&&s[i]=='x'&&s[i-1]==')'){
					s.insert(s.begin()+i,'*');
					len++;
					i++;
				}
			}
			make(s);
			List result=cal();
			if(!mode){
				cout<<result.print()<<endl;
				res.set_content(result.print(),"text/plain");
			}
			else{
				cout<<result.printAsNumbers()<<endl;
				res.set_content(result.printAsNumbers(),"text/plain");
			}

		}
	});
	server.listen("localhost",8080);
	return 0;
}
