#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <set>
#include <stack>
#include <vector>
#include <windows.h>
using namespace std;

//
string guns_name[11]={"","小刀","菜刀","军刀","小枪","冲锋枪","AK","狙","散弹","机关枪","WINNER"}; //枪械id与名称相对应 
// 小刀 +998244353 菜刀 1 军刀 2 
struct guns{
	int id;//枪的编号 
	int AC;//护甲点数，扣除完毕后枪械报废 
	int bullet;//子弹数目，装弹后视为无限子弹
	int toward;//枪口朝向，远程武器初值为0，近程武器赋值为-1表示不可远瞄
	int short_gun;//近程武器初值为伤害值，远程为-1，其中只有AK和机关枪远近都能
	int long_gun;//远程武器初值为伤害值，近程为-1，其中只有AK和机关枪远近都能
	void operator = (guns y) {
		id=y.id;
		AC=y.AC;
		bullet=y.bullet;
		toward=y.toward;
		short_gun=y.short_gun;
		long_gun=y.long_gun;
	}
	guns(){
		id=0;
		AC=0;
		bullet=0;
		toward=-1;
		short_gun=-1;
		long_gun=-1;
	}
	guns(int a1,int a2,int a3,int a4,int a5,int a6){
		id=a1;
		AC=a2;
		bullet=a3;
		toward=a4;
		short_gun=a5;
		long_gun=a6;
	}
}gun0[7];
/*
	gun0[1]=guns(4,1,0,0,-1,1);//甲1远1 
	gun0[2]=guns(5,1,0,-1,1,-1);//甲1近1 
	gun0[3]=guns(6,2,0,0,2,2);//甲2近2远2 
	gun0[4]=guns(7,3,0,0,-1,3);//甲3远3 
	gun0[5]=guns(8,3,0,-1,3,-1);//甲3近3 
	gun0[6]=guns(9,3,0,0,3,3);//甲3近3远3 
	*/
string user_name;
struct players{
	int id;   //编号
	string name;//名称
	int player_class;//用于区别玩家和不同类别的人机,人类为1,普通人机为2，高级为3 
	int level;//级别 
	int knives[5];//1小刀2菜刀3军刀，用于记录数量（刀没有上限）
	guns gun[4];//用于存储枪械，最多持有3把
				//护甲值优先扣除后持有的武器
	int gun_num;//记录枪械个数 
	int khp;//特判刀类武器血量2
	int ghp;//特判枪类武器血量1
	int life;//判定存活状态
	bool trousers;//小刀特判裤子，初值赋为1 
	int home;//家，方位信息 
	int kill;//索敌（AI2，AI3） 
	void operator = (players y){//???
		player_class=y.player_class; 
		id=y.id;
		name=y.name;
		level=y.level;
		memcpy(knives,y.knives,sizeof(knives));
		memcpy(gun,y.gun,sizeof(gun));
	//	memcpy(knives,y.knives,sizeof(knives));
		gun_num=y.gun_num;
		khp=y.khp;
		ghp=y.ghp;
		life=y.life;
		trousers=y.trousers;
		home=id; 
		kill=y.kill; 
	} 
	players(){
		id=0;
		name="默认名称";
		level=1;
		memset(knives,0,sizeof(knives)); 
		memset(gun,0,sizeof(gun));
		gun_num=0;
		khp=2;
		ghp=1;
		life=1;
		trousers=1; 
		player_class=2;
		home=id;
		kill=0;
	}
	players(int a1,string a2,int a3,int a4){//用于每局游戏初始化 
		id=a1;
		name=a2;
		level=a3;
		player_class=a4;
		memset(knives,0,sizeof(knives)); 
		memset(gun,0,sizeof(gun));
		gun_num=0;
		khp=2;
		ghp=1;
		life=1;
		trousers=1; 
		home=id;
		kill=0;
	}
}player[31];
//声明全部函数
void start();//其他不方便的初始化 
void show(int x);//展示指定玩家信息，0为所有 
void show_gun(guns x);//show的附属函数 
//string get_order(int a);
bool life();//只剩一人返回true
int winner();//获胜者id 
void lowest(int win_id);//低保
bool player_move();//依据参数行动 
void test_move();//测试模式，打靶子，重要的调试函数和修改函数 
int get_int();//获取一个在[l,r]范围内的非负整数，0≤l≤r<1
void user_move(bool is_first,bool call_from_second);//获取玩家行动参数
void user_move_second();//user_move()的子函数，方便二次询问时使用return
int who_move();//决定谁行动，返回id 
void AI_stop();//AI执行结束后需等待用户输入回车以继续，重要的后门程序
void kkk();//后门程序1
void sc03();//后门程序2 
bool AI1_move();//依据参数行动 ，没有错误提示
int AI1_random(int a1,int a2,int a3,int a4);//AI1行动类型概率，带权重，依旧很菜
int AI1_random2(int x);//移动改判，闪避下调至1/3，提裤子上调2/3 
void AI1();//AI1行动参数 
int up(int x,int y);//除法上取整
int check2(int x,int y);//AI2专用，x用已有武器击杀y所需步数
int AI2_step(int y);//选取步数最少的武器，返回参数 
void AI2_move(int k,int y);//AI2专用，用k武器攻击y，自动执行相应步骤 
void AI2();//AI2行动参数，尽量高级武器优先，步数少者优先，10%索敌 
int check3(int x,int y,int z);//x击杀y通用计算函数！！！！！////z 用于补充护甲计算 
int search_kill(int x);//AI3索敌  
int AI3_step(int y);//AI3专用，选取步数最少的武器，返回参数
void AI3_move(int k,int y);//AI3专用，k武器攻击y 
void AI3();//随机锁定除自己以外的最高等级，直到对方死亡 
void new_game();//一场新的游戏 
void GAME();//游戏主函数
 
void start(){//其他不方便的初始化 
	printf("尊姓大名：");
	getline(cin,user_name);
	for(int i=1;i<=30;i++){
		player[i].id=i;
		char c[2];
		c[0]=i+'0';
		c[1]=0;
		string s=c;
		s="id"+s;
		player[i].name=s;
	} 
	player[1].name=user_name;
}
///////游戏内相关 ////////////////////
/*
string get_order(int a){
	string s;
	if(a==0){
		cin>>s;
	}
	if(a==1){
		printf("请输入指令：");
		cin>>s;
	}
	if(a==2){
		printf("Debug\n");
	}
	
	return s;
	
}*/
int player_num;//玩家数 
void show_gun(guns x){//
	cout<<guns_name[x.id]<<"  ";
	printf("剩余护甲：%d  ",x.AC);
	if(x.bullet) printf("已装弹  ");
	else printf("未装弹  ");
	if(x.toward>0) printf("瞄准了：%d\n",x.toward);
	else printf("\n");
	return;
}
void show(int x){//展示指定玩家信息，0为所有 
	if(!x){
		for(int i=1;i<=player_num;i++){
			show(i);
		}
		for(int i=1;i<=player_num;i++){
			cout<<i<<"房:";
			for(int j=1;j<=player_num;j++){
				if(player[j].life>0&&player[j].home==i) cout<<j<<" ";
			} 
			cout<<" === ";
			//cout<<endl;
		} 
		cout<<endl;
	}
	else{
		printf("ID:%d ",player[x].id);printf("name:");cout<<player[x].name<<"  ";
		if(player[x].life){
			printf("级别：%d",player[x].level);cout<<guns_name[player[x].level]<<"  ";
			printf("位于%d\n",player[x].home);
			if(player[x].trousers) printf("裤子存在  ");
			else printf("裤子不存在  ");
			if(player[x].khp==1) printf("半血\n");
			else printf("满血\n");
			if(player[x].knives[1]) printf("小刀%d把\n",player[x].knives[1]);
			if(player[x].knives[2]) printf("菜刀%d把\n",player[x].knives[2]);
			if(player[x].knives[3]) printf("军刀%d把\n",player[x].knives[3]);
			for(int i=1;i<=player[x].gun_num;i++){
				show_gun(player[x].gun[i]);
			} 
		}
		else{
			printf("已阵亡\n");
		}
	}
	return; 
}
bool life(){//只剩一人返回true 
	int a=0;
	for(int i=1;i<=player_num;i++){
		if(player[i].life) a++;
	}
	if(a==1) return true;
	return false;
}
int winner(){//获胜者id 
	for(int i=1;i<=player_num;i++){
		if(player[i].life) return i;
	}
	return 0;
} 
void lowest(int win_id){//低保
	cout<<"玩家"<<player[win_id].name<<"本回合获胜，当前他持有的武器是:";
	cout<<guns_name[player[win_id].level]<<endl;
	int minn=9,maxx=0;
	for(int i=1;i<=player_num;i++){
		minn=min(minn,player[i].level);
		maxx=max(maxx,player[i].level);
	}
	if(maxx>minn+3){
		minn=maxx-4;
		printf("当前最高级别武器为：");
		cout<<guns_name[maxx]<<endl;
		printf("所有玩家自动提升至不低于：");
		cout<<guns_name[minn+1]<<endl; 
		for(int i=1;i<=player_num;i++){
			player[i].level=max(player[i].level,minn+1); 
		}
	}
	return;
}
///行动参数
int the_move;//行动人id 
int m_struct;//行动类型
int m_2;//第2参数 
int m_3;//第3参数
/*行动类型：1移动 		第2参数：家的id
			2获取武器	第2参数：武器类型编号
			3装填		第2参数：枪械类型编号(默认填充更早持有的枪械）
			4瞄准		第2参数：枪械位置编号（123）			第3参数：瞄准对象 
			5攻击		第2参数：武器类型编号&123(2位数掩码）	第3参数：攻击对象(自动进行相应行动）
			6抢夺		第2参数：武器编号						第3参数：抢夺对象（暂且不写）	
			7拾取
			0show 
*/
// 
/// 
bool player_move(){//依据参数行动 
	int x=the_move;
	if(m_struct==1){
		if(m_2>player_num){
			printf("您不能无中生'家'\n");
			return true;
		}
		else if(m_2<0){
			printf("您是想去地狱转转？\n");
			return true;
		}
		else if(m_2==0){//新增操作-提裤子//
			if(player[x].trousers>0){
				printf("您穿着裤子，不能再买一条新的穿上\n");
				return true;
			}
			else{
				cout<<x<<"号玩家"<<player[x].name<<"小心翼翼的提上了自己的裤子\n";
				player[x].trousers=1;
				return false; 
			}
		}
		else if(m_2==player[x].home){
			for(int i=1;i<=player_num;i++){//闪避他人瞄准 
				for(int j=1;j<=player[i].gun_num;j++){
					if(player[i].gun[j].toward==x){
						player[i].gun[j].toward=0;
					}
				}
			} 
			cout<<x<<"号玩家"<<player[x].name<<"闪避了所有的瞄准\n";
			return false;
		}
		else{
			cout<<x<<"号玩家"<<player[x].name<<" 移动到 "<<m_2<<"号房间\n";
			player[x].home=m_2;
			for(int i=1;i<=player[x].gun_num;i++){//移动后自己瞄准失效 
				if(player[x].gun[i].toward>0){
					player[x].gun[i].toward=0;
				}
			} 
			for(int i=1;i<=player_num;i++){//闪避他人瞄准 
				for(int j=1;j<=player[i].gun_num;j++){
					if(player[i].gun[j].toward==x){
						player[i].gun[j].toward=0;
					}
				}
			} 
			return false;
		}
		printf("移动的未知错误\n");
		return true;
	}
	else if(m_struct==2){
		if(m_2>player[x].level){
			printf("不能取得比自己等级更高的武器\n"); 
			return true;
		}
		else if(m_2>=1&&m_2<=3){
			player[x].knives[m_2]++;
			cout<<x<<"号玩家"<<player[x].name<<" 持有了 "<<guns_name[m_2]<<"\n";
			return false;
		}
		else if(m_2>=4&&m_2<=9){
			if(player[x].gun_num>=3){
				printf("枪械数量达到上限\n");
				return true;
			}
			else{
				player[x].gun[++player[x].gun_num]=gun0[m_2-3];
				cout<<x<<"号玩家"<<player[x].name<<" 持有了 "<<guns_name[m_2]<<"\n";
				return false;
			}
		}
		else{
			printf("非法持枪/刀\n");
			return true; 
		}
		printf("持有的未知错误\n"); 
		return true;
	}
	else if(m_struct==3){
		for(int i=1;i<=player[x].gun_num;i++){
			if(player[x].gun[i].id==m_2&&player[x].gun[i].bullet<=0){
				cout<<x<<"号玩家"<<player[x].name<<" 对第 "<<i<<"把枪-"<<guns_name[m_2]<<"-装填了子弹\n";
				player[x].gun[i].bullet=32765;
				return false; 
			}
		} 
		printf("装填参数有误，请检查枪械是否存在或已经被装填\n");
		return true;
	}
	else if(m_struct==4){
		if(!(m_3>=1&&m_3<=player_num)){
			printf("禁止造人\n");
			return true; 
		} 
		if(!player[m_3].life){
			printf("禁止鞭尸\n");
			return true;
		}
		/*if(m_2>=1&&m_2<=3){
			printf("刀类没有瞄准\n");
			return true; 
		}
		if(m_2>9||m_2<=0){
			printf("不存在该类别武器\n");
			return true;
		}
		if(gun0[m_2].long_gun==-1){
			printf("近程类别武器不能瞄准\n");
			return true;
		}
		*/
		if(player[x].gun[m_2].toward>=0&&m_2<=player[x].gun_num&&m_2>0){
			cout<<x<<"号玩家"<<player[x].name<<" 将第 "<<m_2<<"把枪-"<<guns_name[player[x].gun[m_2].id]<<"-瞄准了";//分成两行好看 
			cout<<m_3<<"号玩家"<<player[m_3].name<<"\n";
			player[x].gun[m_2].toward=m_3; 
			return false; 
		}
		printf("瞄准参数有误，请检查是否有该武器,或近程类别武器不能瞄准\n");
		return true;
	}
	else if(m_struct==5){
		if(!player[m_3].life){
			printf("禁止鞭尸\n");
			return true;
		}
		if(!(m_3>=1&&m_3<=player_num)){
			printf("禁止造人\n");
			return true; 
		}
		if(m_3==x){
			printf("调戏对面要适度哦\n");//正常执行下面 
		}
		if(m_2%10==1&&m_2/10==1){
			m_2%=10;
			if(!player[x].knives[1]){
				cout<<"请先持有"<<guns_name[1]<<endl;
				return true;
			}
			if(player[x].home!=player[m_3].home){
				printf("近战攻击必须要共处一室哦~\n");
				return true;
			}
			if(player[m_3].trousers>0){
				if(m_3==x){
					cout<<x<<"号玩家"<<player[x].name<<"脱下了自己的裤子，并表示你们太弱了\n";
					player[x].trousers=0;
				}
				else{
					cout<<player[x].name<<" 扒下了 ";//分成两行好看 
					cout<<m_3<<"号玩家"<<player[m_3].name<<" 的裤衩子\n";
					player[m_3].trousers=0;
				}
				return false;
				
			}
			else{
				cout<<x<<"号玩家"<<player[x].name<<" 使用-"<<guns_name[m_2]<<"-击杀了";//分成两行好看 
				cout<<m_3<<"号玩家"<<player[m_3].name<<"\n";
				player[m_3].life=0; 
				return false;
			}
			printf("小刀bug\n");
			return true;
			
		}
		
		else if(m_2/10==1&&(m_2%10==2||m_2%10==3)){
			m_2%=10;
			int hurt=0;
			if(!player[x].knives[m_2]){
				cout<<"请先持有"<<guns_name[m_2]<<endl;
				return true; 
			}
			if(player[x].home!=player[m_3].home){
				printf("近战攻击必须要共处一室哦~\n");
				return true;
			}
			hurt=m_2-1;
			cout<<x<<"号玩家"<<player[x].name<<" 使用-"<<guns_name[m_2]<<"-攻击";//分成两行好看 
			cout<<m_3<<"号玩家"<<player[m_3].name<<"\n";
			
			for(int i=player[m_3].gun_num;i>=1;i--){
				if(hurt>=player[m_3].gun[i].AC){
					hurt-=player[m_3].gun[i].AC;
					player[m_3].gun[i].AC=0;
					cout<<m_3<<"号玩家"<<player[m_3].name<<"第"<<i<<"把枪-"<<guns_name[player[m_3].gun[i].id]<<"-被击落\n";
					player[m_3].gun_num--;
				}
				else{
					player[m_3].gun[i].AC-=hurt;
					hurt=0;
					break;
				}
				
			}
			cout<<m_3<<"号玩家"<<player[m_3].name<<"受到"<<hurt<<"点刀伤"<<"\n";
			player[m_3].khp-=hurt;
			if(player[m_3].khp<=0){
				cout<<x<<"号玩家"<<player[x].name<<" 使用-"<<guns_name[m_2]<<"-击杀了";//分成两行好看 
				cout<<m_3<<"号玩家"<<player[m_3].name<<"\n";
				player[m_3].life=0; 
			} 
			return false;
			//printf("刀bug\n");
			//return true;
		}
		else if(m_2/10==2){
			m_2%=10;
			if(!(m_2>=1&&m_2<=player[x].gun_num)){
				printf("武器不存在\n");
				return true;
			}
			if(player[x].gun[m_2].bullet<=0){
				printf("枪械未装填子弹\n");
				return true;
			}
			//无论如何，开枪后枪口朝向（瞄准）失效 
			if(player[x].gun[m_2].short_gun>0&&player[x].home==player[m_3].home){
				int hurt=player[x].gun[m_2].short_gun;
				int xxx=player[x].gun[m_2].id;
				cout<<x<<"号玩家"<<player[x].name<<" 使用-"<<guns_name[xxx]<<"-近程攻击";//分成两行好看 
				cout<<m_3<<"号玩家"<<player[m_3].name<<"\n";
				for(int i=player[m_3].gun_num;i>=1;i--){
					if(hurt>=player[m_3].gun[i].AC){
						hurt-=player[m_3].gun[i].AC;
						player[m_3].gun[i].AC=0;
						cout<<m_3<<"号玩家"<<player[m_3].name<<"第"<<i<<"把枪-"<<guns_name[player[m_3].gun[i].id]<<"-被击落\n";
						player[m_3].gun_num--;
					}
					else{
						player[m_3].gun[i].AC-=hurt;
						hurt=0;
						break;
					}
					
				}
				cout<<m_3<<"号玩家"<<player[m_3].name<<"受到"<<hurt<<"点枪伤"<<"\n";
				player[m_3].ghp-=hurt;
				if(player[m_3].ghp<=0){
					cout<<x<<"号玩家"<<player[x].name<<" 使用-"<<guns_name[xxx]<<"-击杀了";//分成两行好看 
					cout<<m_3<<"号玩家"<<player[m_3].name<<"\n";
					player[m_3].life=0; 
				} 
				if(player[x].gun[m_2].toward>0){
					player[x].gun[m_2].toward=0;
					printf("该枪的枪口瞄准已失效\n");
				}
				return false;
			}
			else if(player[x].gun[m_2].toward==m_3){
				int hurt=player[x].gun[m_2].long_gun;
				int xxx=player[x].gun[m_2].id;
				cout<<x<<"号玩家"<<player[x].name<<" 使用-"<<guns_name[xxx]<<"-远程攻击";//分成两行好看 
				cout<<m_3<<"号玩家"<<player[m_3].name<<"\n";
				for(int i=player[m_3].gun_num;i>=1;i--){
					if(hurt>=player[m_3].gun[i].AC){
						hurt-=player[m_3].gun[i].AC;
						player[m_3].gun[i].AC=0;
						cout<<m_3<<"号玩家"<<player[m_3].name<<"第"<<i<<"把枪-"<<guns_name[player[m_3].gun[i].id]<<"-被击落\n";
						player[m_3].gun_num--;
					}
					else{
						player[m_3].gun[i].AC-=hurt;
						hurt=0;
						break;
					}
					
				}
				cout<<m_3<<"号玩家"<<player[m_3].name<<"受到"<<hurt<<"点枪伤"<<"\n";
				player[m_3].ghp-=hurt;
				if(player[m_3].ghp<=0){
					cout<<x<<"号玩家"<<player[x].name<<" 使用-"<<guns_name[xxx]<<"-击杀了";//分成两行好看 
					cout<<m_3<<"号玩家"<<player[m_3].name<<"\n";
					player[m_3].life=0; 
				} 
				if(player[x].gun[m_2].toward>0){
					player[x].gun[m_2].toward=0;
					printf("该枪的枪口瞄准已失效\n");
				}
				return false;
			}
			else{
				printf("远程未瞄准/近程未共处一室\n");
				return true;
			}
		}
		printf("攻击的未知错误\n"); 
		return true;
	}
	else if(m_struct==6){//抢夺 
		printf("该功能还没有开放，换一个行动试试\n");
		return true; 
	}
	else if(m_struct==7){//拾取 
		printf("该功能还没有开放，换一个行动试试\n");
		return true;
	}
	else{
		m_struct=m_2=m_3=0;
		printf("行动类型参数有误\n");
		return true; 
	}
	
}
void test_move(){//测试模式，打靶子 
	printf("请输入指令：");
	cin>>m_struct>>m_2>>m_3;
	if(m_struct==0){
		show(m_2);
	}
	else{
		player_move();
	}
}
////用户界面 
int get_int(int l,int r){//获取一个在[l,r]范围内的非负整数，0≤l≤r<10
	string player_input;
	int in=-1;
	string back="return\0";
	do{
		cin>>player_input;
		if(player_input==back) return -1;
		if(player_input.length()>1)
		{
			printf("输入不合法，请重新输入：");
			continue;
		}
		in=player_input[0]-'0';
		if(in<l||in>r) printf("输入不合法，请重新输入：");
	}while(in<l||in>r);
	getchar();
	return in;
}
int move_stop;//防止因return而多次调用user_move
void user_move(bool is_first,bool call_from_second=0){//获取玩家行动参数 
	if(!call_from_second){//若从second来，则跳过此部分
		if(is_first) printf("该 %s 行动了，请输入行动方式前的正整数。\n",player[the_move].name.c_str());
		printf("0.展示玩家信息（此行动不会消耗您的行动机会）\n");
		printf("1.移动/闪避/提上裤子\n");
		printf("2.获取武器\n");
		printf("3.装填子弹\n");
		printf("4.瞄准\n");
		printf("5.攻击\n");
	//	printf("6.抢夺/捡拾武器\n");
		int player_input;
		player_input=get_int(0,5);
		m_struct=player_input;
		move_stop=0;//防止多次调用 
		user_move_second();
	}
	if(move_stop) return;
	int l,r;
	switch(m_struct){//根据第一参数决定返回值或继续询?
		//0已经写过了
		case 4:{
			printf("本局共有%d个玩家，您正在使用\"%s\"进行瞄准，请输入瞄准对象的编号：\n",player_num,guns_name[player[the_move].gun[m_2].id].c_str());
			l=1,r=player_num;
		}break;
		case 5:{
			int cnt=0,kind_weapon=0;//kind存武器类型
			for(int i=1;i<=3;++i){//不想在函数间传数组了，重新搜一遍武器排序
				if(player[the_move].knives[i]){
					++cnt;
					if(cnt==m_2){//说明用户选择的是刀类
						m_2=10;//m_2的第一位为1表示刀，为2表示枪
						m_2+=i;
						kind_weapon=1;
					}
				}
			}
			if(kind_weapon==0){//下面是选择为枪类的情况，直接相减就行
				kind_weapon=2;
				m_2-=cnt;
				m_2+=20;//写到这里2022年7月21日21:34:24
				printf("本局共有%d个玩家，您正在使用\"%s\"进行攻击，请输入攻击对象的编号：\n",player_num,guns_name[player[the_move].gun[m_2%10].id].c_str());
			}
			else printf("本局共有%d个玩家，您正在使用\"%s\"进行攻击，请输入攻击对象的编号：\n",player_num,guns_name[m_2%10].c_str());
			l=1,r=player_num;
		}break;
		default:{//123都是直接为player_move()传参数
			bool judge=player_move();//判断行动是否合法，不合法重来
			if(judge){
				user_move(0);
				return;
			}return;//参数在user_move_second中已经更改
		}break;
	}
	printf("或者输入\"return\"返回上级\n");
	int input_third;
	input_third=get_int(l,r);//获得第三参数
	if(input_third==-1){//return
		user_move_second();
		return;
	}
	m_3=input_third;
	bool judge=player_move();//判断行动是否合法，不合法重来
	if(judge){
		user_move(0);
		return;
	}
	return;
}
void user_move_second(){//user_move()的子函数，方便二次询问时使用return
	int l,r;//表示二次询问所需值的范围
	int guns_kind[10];//见case3。bz，是否拥有id为i的武器
	switch(m_struct){//根据输入的选项来二次询问
		case 0:{
			printf("本局共有%d个玩家，请输入您要展示信息的玩家编号（或者输入\"0\"来查看全部）：\n",player_num);
			l=0,r=player_num;
		}break;
		case 1:{
			printf("本局共有%d个玩家，请输入您要移动到的房间编号：\n",player_num);
			printf("若您选择移动到自身所在的位置，则视为闪避掉所有其它玩家的瞄准\n");
			printf("若您输入了\"0\"，则视为提上裤子\n");
			l=0,r=player_num;
		}break;
		case 2:{
			printf("您现在%d级，可以获得的武器有：\n",player[the_move].level);
			for(int i=1;i<=player[the_move].level;++i) printf("%d.%s\n",i,guns_name[i].c_str());
			l=1,r=player[the_move].level;
		}break;
		case 3:{
			int t=player[the_move].gun_num;
			if(!t){
				printf("您现在没有已装备的枪械类武器，无法装填子弹，即将返回选择界面......\n");
				Sleep(100);
				user_move(0);
				move_stop=1;//防止因为return而多次调用函数player_move 
				return;
			}
			printf("您现在有下列种类的武器，请选择一种进行装填（只会装填该种武器中最早获得的一个）：\n");
			//int guns_kind[10];//bz，是否拥有id为i的武器
			//因为方便后面用，移到switch外面
			memset(guns_kind,0,sizeof(guns_kind));
			for(int i=1;i<=t;++i) guns_kind[player[the_move].gun[i].id]=1;
			int cnt=0;
			for(int i=1;i<=9;++i){//打印武器列表
				if(guns_kind[i]){
					++cnt;
					printf("%d.%s\n",cnt,guns_name[i].c_str());
				}
			}
			l=1,r=cnt;
		}break;
		case 4:{
			int t=player[the_move].gun_num;
			if(!t){
				printf("您现在没有已装备的枪械类武器，无法瞄准，即将返回选择界面......\n");
				Sleep(100);
				user_move(0);
				move_stop=1;//防止因为return而多次调用函数player_move 
				return;
			}
			printf("您现在有下列枪械类武器，请选择一个进行瞄准：\n");//打印武器列表
			for(int i=1;i<=t;++i){
				printf("%d.%s\n",i,guns_name[player[the_move].gun[i].id].c_str());
			}
			l=1,r=t;
		}break;
		case 5:{
			int cnt=0;
			int t=player[the_move].gun_num;
			if(player[the_move].knives[1]==0&&player[the_move].knives[2]==0&&player[the_move].knives[3]==0&&t==0){
				printf("您现在没有任何武器，无法攻击，即将返回选择界面......\n");
				Sleep(100);
				user_move(0);
				move_stop=1;//防止因为return而多次调用函数player_move 
				return;
			}
			printf("您现在有下列武器，请选择一个进行攻击：\n");//打印武器列表
			for(int i=1;i<=3;++i){
				if(player[the_move].knives[i]){
					++cnt;
					printf("%d.%s\n",cnt,guns_name[i].c_str());
				}
			}
			for(int i=1;i<=t;++i){
				++cnt;
				printf("%d.%s\n",cnt,guns_name[player[the_move].gun[i].id].c_str());
			}
			l=1,r=cnt;
		}break;
		default:{//在错误的时间输入了return
			printf("无法继续return了哦！\n");
			user_move(1);
			move_stop=1;//防止因为return而多次调用函数player_move 
			return;
		}break;
	}
	printf("或者输入\"return\"返回上级\n");
	int input_second;
	input_second=get_int(l,r);//获得第二参数
	if(input_second==-1){//return
		user_move(0);
		move_stop=1;//防止因为return而多次调用函数player_move 
		return;
	}
	if(m_struct==0){//展示后重新询问
		show(input_second);
		user_move(0);
		move_stop=1;//防止因为return而多次调用函数player_move 
		return;
	}
	if(m_struct!=3) m_2=input_second;//传第二参数,装填要特殊处理 
	else{//计算最早的某类型枪的id 
		int cnt=0;
		for(int i=1;i<=9;++i){//排着扫一遍，获得用户输入的枪的类型
			if(guns_kind[i]){
				cnt++;
				if(cnt==input_second){
					m_2=i;
					return;	
				}
			}
		}
	}
	user_move(0,1);
	move_stop=1;//防止因为return而多次调用函数player_move 
	return;
}
/////// 
int who_move(){//决定谁行动，返回id 
	int mm=rand()%(player_num*4)/4+1;
	while(!player[mm].life){
		mm=rand()%(player_num*4)/4+1;
	}
	return mm;
}
///////AI相关////////////////////////
void AI_stop(){//AI执行结束后需等待用户输入回车以继续
	printf("==请确认人机的执行步骤，并回车使游戏继续进行==");
	char c='a';
	while(1){
		c=getchar();
		if(c=='-') show(0);
		if(c=='j'){
			kkk();sc03();
		}
		if(c=='\n') break;
	} 
	return;
}
void kkk(){//后门程序1,展示索敌 
	for(int i=1;i<=player_num;i++){
		if(player[i].player_class==3||player[i].player_class==4){
			if(player[player[i].kill].life>=0&&player[i].kill){
				cout<<i<<"号玩家人机类别"<<player[i].player_class-1<<"锁定了"<<player[i].kill<<endl;
			}
		}
	} 
}
void sc03(){//后门程序2，修改参数 
	int xxx,x1,x2,x3;
	cin>>xxx;
	if(xxx==0){
		return;
	} 
	else if(xxx==1){//可行动修改的参数
		cin>>x1;
		the_move=x1; 
		test_move();
	}
	else if(xxx==2){//level
		cin>>x1>>x2;
		player[x1].level=x2;
		printf("%d号玩家级别更改为%d\n",x1,x2);
	}
	else if(xxx==3){//刀伤恢复 
		cin>>x1;
		player[x1].khp=2;
		printf("%d号玩家已痊愈\n",x1);
	}
	else if(xxx==4){//复活 
		cin>>x1;
		player[x1].life=1;
		printf("%d号玩家已复活\n",x1);
	}
	else if(xxx==5){//修改人机类别 
		cin>>x1>>x2;
		player[x1].player_class=x2+1;
		printf("%d号玩家人机类别更改为%d\n",x1,x2);
	}
	else if(xxx==6){//修改索敌 
		cin>>x1>>x2;
		player[x1].kill=x2;
		printf("%d号玩家人机索敌改为%d\n",x1,x2); 
	}
	printf("==请确认修改步骤，并输入\"=\"使游戏继续进行==");
	char c='a';
	while(1){
		c=getchar();
		if(c=='-') show(0);
		if(c=='j'){
			sc03();
			break;
		}
		if(c=='=') break;
	} 
	return;
}
///预留 

/// 1
bool AI1_move(){//依据参数行动 
	int x=the_move;
	if(m_struct==1){
		if(m_2>player_num){
		//	printf("您不能无中生'家'\n");
			return true;
		}
		else if(m_2<0){
		//	printf("您是想去地狱转转？\n");
			return true;
		}
		else if(m_2==0){//新增操作-提裤子//
			if(player[x].trousers>0){
		//		printf("您穿着裤子，不能再买一条新的穿上\n");
				return true;
			}
			else{
				cout<<x<<"号玩家"<<player[x].name<<"小心翼翼的提上了自己的裤子\n";
				player[x].trousers=1;
				return false; 
			}
		}
		else if(m_2==player[x].home){
			for(int i=1;i<=player_num;i++){//闪避他人瞄准 
				for(int j=1;j<=player[i].gun_num;j++){
					if(player[i].gun[j].toward==x){
						player[i].gun[j].toward=0;
					}
				}
			} 
			cout<<x<<"号玩家"<<player[x].name<<"闪避了所有的瞄准\n";
			return false;
		}
		else{
			cout<<x<<"号玩家"<<player[x].name<<" 移动到 "<<m_2<<"号房间\n";
			player[x].home=m_2;
			for(int i=1;i<=player[x].gun_num;i++){//移动后自己瞄准失效 
				if(player[x].gun[i].toward>0){
					player[x].gun[i].toward=0;
				}
			} 
			for(int i=1;i<=player_num;i++){//闪避他人瞄准 
				for(int j=1;j<=player[i].gun_num;j++){
					if(player[i].gun[j].toward==x){
						player[i].gun[j].toward=0;
					}
				}
			} 
			return false;
		}
	//	printf("移动的未知错误\n");
		return true;
	}
	else if(m_struct==2){
		if(m_2>player[x].level){
	//		printf("不能取得比自己等级更高的武器\n"); 
			return true;
		}
		else if(m_2>=1&&m_2<=3){
			if(player[x].knives[m_2]) return true;//防止AI1重复拿武器-刀 
			player[x].knives[m_2]++;
			cout<<x<<"号玩家"<<player[x].name<<" 持有了 "<<guns_name[m_2]<<"\n";
			return false;
		}
		else if(m_2>=4&&m_2<=9){
			if(player[x].gun_num>=3){
	//			printf("枪械数量达到上限\n");
				return true;
			}
			else{
				player[x].gun[++player[x].gun_num]=gun0[m_2-3];
				cout<<x<<"号玩家"<<player[x].name<<" 持有了 "<<guns_name[m_2]<<"\n";
				return false;
			}
		}
		else{
		//	printf("非法持枪/刀\n");
			return true; 
		}
	//	printf("持有的未知错误\n"); 
		return true;
	}
	else if(m_struct==3){
		for(int i=1;i<=player[x].gun_num;i++){
			if(player[x].gun[i].id==m_2&&player[x].gun[i].bullet<=0){
				cout<<x<<"号玩家"<<player[x].name<<" 对第 "<<i<<"把枪-"<<guns_name[m_2]<<"-装填了子弹\n";
				player[x].gun[i].bullet=32765;
				return false; 
			}
		} 
	//	printf("装填参数有误，请检查枪械是否存在或已经被装填\n");
		return true;
	}
	else if(m_struct==4){
		if(!(m_3>=1&&m_3<=player_num)){
	//		printf("禁止造人\n");
			return true; 
		} 
		if(!player[m_3].life){
	//		printf("禁止鞭尸\n");
			return true;
		}
		if(m_3==x){
		//	printf("调戏对面要适度哦\n");//正常执行下面 
			return true;//防止AI1自刀 
		}
		/*if(m_2>=1&&m_2<=3){
			printf("刀类没有瞄准\n");
			return true; 
		}
		if(m_2>9||m_2<=0){
			printf("不存在该类别武器\n");
			return true;
		}
		if(gun0[m_2].long_gun==-1){
			printf("近程类别武器不能瞄准\n");
			return true;
		}
		*/
		if(player[x].gun[m_2].toward>=0&&m_2<=player[x].gun_num&&m_2>0){
			cout<<x<<"号玩家"<<player[x].name<<" 将第 "<<m_2<<"把枪-"<<guns_name[player[x].gun[m_2].id]<<"-瞄准了";//分成两行好看 
			cout<<m_3<<"号玩家"<<player[m_3].name<<"\n";
			player[x].gun[m_2].toward=m_3; 
			return false; 
		}
	//	printf("瞄准参数有误，请检查是否有该武器,或近程类别武器不能瞄准\n");
		return true;
	}
	else if(m_struct==5){
		if(!player[m_3].life){
	//		printf("禁止鞭尸\n");
			return true;
		}
		if(!(m_3>=1&&m_3<=player_num)){
	//		printf("禁止造人\n");
			return true; 
		}
		if(m_3==x){
		//	printf("调戏对面要适度哦\n");//正常执行下面 
			return true;//防止AI1自刀 
		}
		if(m_2%10==1&&m_2/10==1){
			m_2%=10;
			if(!player[x].knives[1]){
			//	cout<<"请先持有"<<guns_name[1]<<endl;
				return true;
			}
			if(player[x].home!=player[m_3].home){
			//	printf("近战攻击必须要共处一室哦~\n");
				return true;
			}
			if(player[m_3].trousers>0){
				if(m_3==x){
					cout<<x<<"号玩家"<<player[x].name<<"脱下了自己的裤子，并表示你们太弱了\n";
					player[x].trousers=0;
				}
				else{
					cout<<player[x].name<<" 扒下了 ";//分成两行好看 
					cout<<m_3<<"号玩家"<<player[m_3].name<<" 的裤衩子\n";
					player[m_3].trousers=0;
				}
				return false;
				
			}
			else{
				cout<<x<<"号玩家"<<player[x].name<<" 使用-"<<guns_name[m_2]<<"-击杀了";//分成两行好看 
				cout<<m_3<<"号玩家"<<player[m_3].name<<"\n";
				player[m_3].life=0; 
				return false;
			}
			printf("小刀bug\n");
			return true;
			
		}
		
		else if(m_2/10==1&&(m_2%10==2||m_2%10==3)){
			m_2%=10;
			int hurt=0;
			if(!player[x].knives[m_2]){
		//		cout<<"请先持有"<<guns_name[m_2]<<endl;
				return true; 
			}
			if(player[x].home!=player[m_3].home){
		//		printf("近战攻击必须要共处一室哦~\n");
				return true;
			}
			hurt=m_2-1;
			cout<<x<<"号玩家"<<player[x].name<<" 使用-"<<guns_name[m_2]<<"-攻击";//分成两行好看 
			cout<<m_3<<"号玩家"<<player[m_3].name<<"\n";
			
			for(int i=player[m_3].gun_num;i>=1;i--){
				if(hurt>=player[m_3].gun[i].AC){
					hurt-=player[m_3].gun[i].AC;
					player[m_3].gun[i].AC=0;
					cout<<m_3<<"号玩家"<<player[m_3].name<<"第"<<i<<"把枪-"<<guns_name[player[m_3].gun[i].id]<<"-被击落\n";
					player[m_3].gun_num--;
				}
				else{
					player[m_3].gun[i].AC-=hurt;
					hurt=0;
					break;
				}
				
			}
			cout<<m_3<<"号玩家"<<player[m_3].name<<"受到"<<hurt<<"点刀伤"<<"\n";
			player[m_3].khp-=hurt;
			if(player[m_3].khp<=0){
				cout<<x<<"号玩家"<<player[x].name<<" 使用-"<<guns_name[m_2]<<"-击杀了";//分成两行好看 
				cout<<m_3<<"号玩家"<<player[m_3].name<<"\n";
				player[m_3].life=0; 
			} 
			return false;
			//printf("刀bug\n");
			//return true;
		}
		else if(m_2/10==2){
			m_2%=10;
			if(!(m_2>=1&&m_2<=player[x].gun_num)){
		//		printf("武器不存在\n");
				return true;
			}
			if(player[x].gun[m_2].bullet<=0){
		//		printf("枪械未装填子弹\n");
				return true;
			}
			//无论如何，开枪后枪口朝向（瞄准）失效 
			if(player[x].gun[m_2].short_gun>0&&player[x].home==player[m_3].home){
				int hurt=player[x].gun[m_2].short_gun;
				int xxx=player[x].gun[m_2].id;
				cout<<x<<"号玩家"<<player[x].name<<" 使用-"<<guns_name[xxx]<<"-近程攻击";//分成两行好看 
				cout<<m_3<<"号玩家"<<player[m_3].name<<"\n";
				for(int i=player[m_3].gun_num;i>=1;i--){
					if(hurt>=player[m_3].gun[i].AC){
						hurt-=player[m_3].gun[i].AC;
						player[m_3].gun[i].AC=0;
						cout<<m_3<<"号玩家"<<player[m_3].name<<"第"<<i<<"把枪-"<<guns_name[player[m_3].gun[i].id]<<"-被击落\n";
						player[m_3].gun_num--;
					}
					else{
						player[m_3].gun[i].AC-=hurt;
						hurt=0;
						break;
					}
					
				}
				cout<<m_3<<"号玩家"<<player[m_3].name<<"受到"<<hurt<<"点枪伤"<<"\n";
				player[m_3].ghp-=hurt;
				if(player[m_3].ghp<=0){
					cout<<x<<"号玩家"<<player[x].name<<" 使用-"<<guns_name[xxx]<<"-击杀了";//分成两行好看 
					cout<<m_3<<"号玩家"<<player[m_3].name<<"\n";
					player[m_3].life=0; 
				} 
				if(player[x].gun[m_2].toward>0){
					player[x].gun[m_2].toward=0;
					printf("该枪的枪口瞄准已失效\n");
				}
				return false;
			}
			else if(player[x].gun[m_2].toward==m_3){
				int hurt=player[x].gun[m_2].long_gun;
				int xxx=player[x].gun[m_2].id;
				cout<<x<<"号玩家"<<player[x].name<<" 使用-"<<guns_name[xxx]<<"-远程攻击";//分成两行好看 
				cout<<m_3<<"号玩家"<<player[m_3].name<<"\n";
				for(int i=player[m_3].gun_num;i>=1;i--){
					if(hurt>=player[m_3].gun[i].AC){
						hurt-=player[m_3].gun[i].AC;
						player[m_3].gun[i].AC=0;
						cout<<m_3<<"号玩家"<<player[m_3].name<<"第"<<i<<"把枪-"<<guns_name[player[m_3].gun[i].id]<<"-被击落\n";
						player[m_3].gun_num--;
					}
					else{
						player[m_3].gun[i].AC-=hurt;
						hurt=0;
						break;
					}
					
				}
				cout<<m_3<<"号玩家"<<player[m_3].name<<"受到"<<hurt<<"点枪伤"<<"\n";
				player[m_3].ghp-=hurt;
				if(player[m_3].ghp<=0){
					cout<<x<<"号玩家"<<player[x].name<<" 使用-"<<guns_name[xxx]<<"-击杀了";//分成两行好看 
					cout<<m_3<<"号玩家"<<player[m_3].name<<"\n";
					player[m_3].life=0; 
				} 
				if(player[x].gun[m_2].toward>0){
					player[x].gun[m_2].toward=0;
					printf("该枪的枪口瞄准已失效\n");
				}
				return false;
			}
			else{
		//		printf("远程未瞄准/近程未共处一室\n");
				return true;
			}
		}
		printf("攻击的未知错误\n"); 
		return true;
	}
	else if(m_struct==6){//抢夺 
		printf("该功能还没有开放，换一个行动试试\n");
		return true; 
	}
	else if(m_struct==7){//拾取 
		printf("该功能还没有开放，换一个行动试试\n");
		return true;
	}
	else{
		m_struct=m_2=m_3=0;
		printf("行动类型参数有误\n");
		return true; 
	}
	
}
int AI1_random(int a1,int a2,int a3,int a4){//AI1行动类型概率 
	int x=rand()%100+1;
	if(x<=a1){
		return 1;
	}
	else if(x<=a2){
		return 2;
	}
	else if(x<=a3){
		return 3;
	}
	else if(x<=a4){
		return 4;
	}
	else if(x<=100){
		return 5;
	}
	else{
		return 0;
	}
}
int AI1_random2(int x){//移动改判 
	int y=rand()%(player_num+1);
	if(y==x){
		if(rand()%3) y=0;
	}
	return y;
}
void AI1(){//AI1行动参数 
	//printf("人机行动回合\n");
	int x=the_move;int cnt=0;
	m_struct=AI1_random(5,25,45,60);
	do{
		cnt++;
		if(cnt>=50){
			cnt=0;
			if(m_struct==1) m_struct=AI1_random(5,30,45,60);
			else if(m_struct==2) m_struct=AI1_random(5,5,30,60);
			else if(m_struct==3) m_struct=AI1_random(20,40,40,60);
			else if(m_struct==4) m_struct=AI1_random(15,40,60,60);
			else if(m_struct==5) m_struct=AI1_random(10,30,60,100);
			
		}
		if(m_struct==1){
			m_2=AI1_random2(player[the_move].home);
		}
		else if(m_struct==2){
			m_2=rand()%player[the_move].level+1;
		}
		else if(m_struct==3){
			m_2=rand()%player[the_move].level+1;
		}
		else if(m_struct==4){
			if(player[the_move].gun_num<=0){
				continue;
			}
			m_2=rand()%player[the_move].gun_num+1;
			m_3=rand()%player_num+1;
		}
		else if(m_struct==5){
			m_2=rand()%2+1;
			m_2=m_2*10+rand()%3+1;
			m_3=rand()%player_num+1;
		}
		else{
			m_struct=0;
			m_2=0;
		}
		
	}while(AI1_move());
} 
///2

	/*int p=0;
	if(player[x].level<=3){
		if(!player[x].knives[player[x].level]){
			m_struct=2;
			m_2=player[x].level;
			p=1;
		}
	}
	else if(player[x].level<=5){
		if(!player[x].knives[3]){
			m_struct=2;
			m_2=3;
			p=1;
		}
		else if(player[x].gun_num<1){
			m_strcut=2;
			m_2=4;
			p=1;
		}
	}
	else if(player[x].level==6){
		if(player[x].gun_num<=1){
			m_strcut=2
			m_2=6;
			p=1;
		}
		else if(!player[x].knives[1]){
			m_strcut=2;
			m_2=1;
			p=1;
		}
	}
	else if(player[x].level==7){
		if(player[x].gun_num==0){
			m_strcut=2
			m_2=6;
			p=1;
		}
		else if(player[x].gun_num==1){
			m_strcut=2
			m_2=7;
			p=1;
		}
		else if(!player[x].knives[1]){
			m_strcut=2;
			m_2=1;
			p=1;
		}
	}*/
///

int up(int x,int y){//除法上取整 
	if(x%y){
		return x/y+1;
	}
	return x/y;
}
int check2(int x,int y){//x用已有武器击杀y所需步数
	if(player[x].level==1){
		//小刀步数判定 
		int step111=100;
		if(player[x].knives[1]) step111=1;
		if(player[x].home!=player[y].home) step111++;
		if(player[y].trousers) step111++;
		return step111;
	}
	else if(player[x].level<=3){
		//刀，步数判定 
		int ac=0,step=100,damage=player[x].level-1;
		for(int i=1;i<=player[y].gun_num;i++){
			ac+=player[y].gun[i].AC;
		}
		step=up(ac+player[y].khp,damage); 
		if(player[x].home!=player[y].home) step++;
		//小刀步数判定 
		int step111=100;
		if(player[x].knives[1]){
			step111=1;
			if(player[x].home!=player[y].home) step111++;
			if(player[y].trousers) step111++;
		} 
		
		
		return min(step,step111);
	}
	else if(player[x].level<=5){
		//刀 
		int ac=0,step=100,damage=2;
		for(int i=1;i<=player[y].gun_num;i++){
			ac+=player[y].gun[i].AC;
		}
		if(player[x].knives[3]){
			step=up(ac+player[y].khp,damage); 
			if(player[x].home!=player[y].home) step++; 
		}
		//小刀 
		int step111=100;
		if(player[x].knives[1]){
			step111=1;
			if(player[x].home!=player[y].home) step111++;
			if(player[y].trousers) step111++;
		}
		int step1=100;
		//枪 
		if(player[x].gun_num>0){
			step1=(ac+1)*2;
			if(player[x].gun[1].toward==y) step1--;
			if(!player[x].gun[1].bullet){
				step1++;
			}
		}
		return min(min(step,step111),step1);
	}
	else{
		int ac=0,step1=100,step2=100,damage;
		for(int i=1;i<=player[y].gun_num;i++){
			ac+=player[y].gun[i].AC;
		}
		if(player[x].gun_num){
			damage=max(player[x].gun[1].long_gun,player[x].gun[1].short_gun);
			if(player[x].gun[1].short_gun>0){
				step1=up(ac+1,damage);
			}
			if(player[x].gun[1].long_gun>0){
				step2=up(ac+1,damage)*2;
				if(player[x].gun[1].toward==y) step2--;
			}
			if(!player[x].gun[1].bullet){
				step1++;step2++;
			}
			if(player[x].home!=player[y].home) step1++;
		}
		
		int step111=100;
		if(player[x].knives[1]){
			step111=1;
			if(player[x].home!=player[y].home) step111++;
			if(player[y].trousers) step111++;
		} 
		return min(min(step1,step2),step111);
		
	}
}
int AI2_step(int y){//选取步数最少的武器，返回掩码 
	int x=the_move;
	if(player[x].level==1){
		return 11;
	}
	else if(player[x].level<=3){
		int ac=0,step=100,damage=player[x].level-1;
		for(int i=1;i<=player[y].gun_num;i++){
			ac+=player[y].gun[i].AC;
		}
		step=up(ac+player[y].khp,damage); 
		if(player[x].home!=player[y].home) step++;
		 
		int step111=100;
		if(player[x].knives[1]){
			step111=1;
			if(player[x].home!=player[y].home) step111++;
			if(player[y].trousers) step111++;
		} 
		if(step111>=step){
			return 10+player[x].level;
		}
		else{
			return 11;
		}
	}
	else if(player[x].level<=5){
		int ac=0,step=100,damage=2;
		for(int i=1;i<=player[y].gun_num;i++){
			ac+=player[y].gun[i].AC;
		}
		step=up(ac+player[y].khp,damage); 
		if(player[x].home!=player[y].home) step++;
		 
		int step111=100;
		if(player[x].knives[1]){
			step111=1;
			if(player[x].home!=player[y].home) step111++;
			if(player[y].trousers) step111++;
		}
		int step1=100;
		if(player[x].gun_num){
			step1=(ac+1)*2;
			if(player[x].gun[1].toward==y) step1--;
			if(!player[x].gun[1].bullet){
				step1++;
			}
		}
		if(min(step,step111)>step1){
			return 21;
		}
		else{
			if(step111>=step){
				return 13;
			}
			else{
				return 11;
			}
		}
		//return min(min(step,step111),step1);
	}
	else{
		int ac=0,step1=100,step2=100,damage;
		for(int i=1;i<=player[y].gun_num;i++){
			ac+=player[y].gun[i].AC;
		}
		if(player[x].gun_num){
			damage=max(player[x].gun[1].long_gun,player[x].gun[1].short_gun);
			if(player[x].gun[1].short_gun>0){
				step1=up(ac+1,damage);
			}
			if(player[x].gun[1].long_gun>0){
				step2=up(ac+1,damage)*2;
				if(player[x].gun[1].toward==y) step2--;
			}
			if(!player[x].gun[1].bullet){
				step1++;step2++;
			}
			if(player[x].home!=player[y].home) step1++;
			
		}
		int step=min(step1,step2); 
		int step111=100;
		if(player[x].knives[1]){
			step111=1;
			if(player[x].home!=player[y].home) step111++;
			if(player[y].trousers) step111++;
		} 
		if(step111>step){
			return 21;
		}
		else{
			return 11;
		}
		
	}
}
///
void AI2_move(int k,int y){
	int x=the_move;
	
	if(k/10==1){//刀 
		k%=10;
		if(player[x].home!=player[y].home){
			m_struct=1;
			m_2=player[y].home;
			//AI1_move();
			player_move();
			return;
		}
		m_struct=5;
		m_2=10+k;
		m_3=y;
		//AI1_move();
		player_move();
		return;
	}
	else{//枪 
		k%=10;
		if(player[x].gun[k].bullet<=0){//装弹判定 
			m_struct=3;
			m_2=player[x].gun[k].id;
			//AI1_move();
			player_move();
			return;
		}
		int ac=0,step1=100,step2=100,damage;
		for(int i=1;i<=player[y].gun_num;i++){
			ac+=player[y].gun[i].AC;
		}
		damage=max(player[x].gun[k].long_gun,player[x].gun[k].short_gun);
		if(player[x].gun[k].short_gun>0){
			step1=up(ac+1,damage);
		}
		if(player[x].gun[k].long_gun>0){
			step2=up(ac+1,damage)*2;
			if(player[x].gun[k].toward==y) step2--;
		}
		if(!player[x].gun[k].bullet){
			step1++;step2++;
		}
		if(player[x].home!=player[y].home) step1++;
		if(step1>step2){
			if(player[x].gun[k].toward!=y){
				m_struct=4;
				m_2=k;
				m_3=y;
				//AI1_move();
				player_move();
				return;
			}
			else{
				m_struct=5;
				m_2=20+k;
				m_3=y;
				//AI1_move();
				player_move();
				return;
			}
		}
		else{
			if(player[x].home!=player[y].home){
				m_struct=1;
				m_2=player[y].home;
				//AI1_move();
				player_move();
				return;
			}
			else{
				m_struct=5;
				m_2=20+k;
				m_3=y;
				//AI1_move();
				player_move();
				return;
			}
		}
	}
} 
void AI2(){//尽量高级武器优先，步数少者优先，10%索敌 
	int x=the_move;
	if(player[player[x].kill].life<=0) player[x].kill=0;
	//优先持有武器，索敌是例外
	if(player[x].kill>0){//索敌特例击杀 
		if(check2(x,player[x].kill)==1){
			AI2_move(AI2_step(player[x].kill),player[x].kill);
			return;
		}
	} 
	if(player[x].level<=3){
		if(!player[x].knives[player[x].level]){
			m_struct=2;
			m_2=player[x].level;
			//AI1_move();
			player_move();
			return;
		}
	}
	else if(player[x].level<=5){
		if(!player[x].knives[3]){
			m_struct=2;
			m_2=3;
			//AI1_move();
			player_move();
			return;
		}
		else if(player[x].gun_num<1){//一击必杀不拿枪 
			int enemy=0,tmp1=100,tmp2;
			for(int i=1;i<=player_num;i++){
				if(i==x) continue;
				if(player[i].life<=0) continue;
				tmp2=check2(x,i);
				if(tmp1>tmp2){
					enemy=i;
					tmp1=tmp2;
				}
				else if(tmp1==tmp2){
					if(rand()%2){
						enemy=i;
						tmp1=tmp2;
					}
				}
				
			}
			if(tmp1>1){
				m_struct=2;
				m_2=4;//只要小枪，冲锋枪综合收益不高 
				//AI1_move();
				player_move();
				return;
			}
			
		}
	}
	else{
		if(player[x].gun_num<=1){
			m_struct=2;
			m_2=player[x].level;
			//AI1_move();
			player_move();
			return;
		}
	}
	if(player[x].kill){
		AI2_move(AI2_step(player[x].kill),player[x].kill);
		return;
	}
	int enemy=0,tmp1=100,tmp2,cntr=2;
	for(int i=1;i<=player_num;i++){
		if(i==x) continue;
		if(player[i].life<=0) continue;
		tmp2=check2(x,i);
		//cout<<i<<"-步数:"<<tmp2<<endl;//::debug
		if(tmp1>tmp2){
			enemy=i;
			tmp1=tmp2;
		}
		else if(tmp1==tmp2){
			if(rand()%cntr==0){
				enemy=i;
				tmp1=tmp2;
			}
			cntr++;
		}
		//cout<<"enemy："<<enemy<<endl; //::debug
		
	}
	//cout<<"步数:"<<tmp1<<endl;//::debug
	//cout<<"攻击："<<enemy<<endl; //::debug
	if(tmp1>1){
		if(!player[x].trousers){//裤子优先 
			m_struct=1;m_2=0;
			//AI1_move();
			player_move();
			return;
		}
	}
	if(tmp1>=4){//转为小刀击杀 
		m_struct=2;
		m_2=1;
		//AI1_move();
		player_move();
		return;
	}
	int the_gun=AI2_step(enemy);
	if(player[x].kill<=0&&rand()%10==0) player[x].kill=enemy;//索敌 
	AI2_move(the_gun,enemy);
	return;
}
///3
///check3及其附带函数 

int check3(int x,int y,int z){//x击杀y通用计算函数！！！！！
////z 用于补充护甲计算  
	int ac=0,step[10];
	ac+=z;
	for(int i=0;i<=9;i++){
		step[i]=100;
	}
	for(int i=1;i<=player[y].gun_num;i++){
		ac+=player[y].gun[i].AC;
	}
	if(player[x].level<=3){
		for(int i=1;i<=player[x].level;i++){
			if(i==1){
				step[1]=1;
				if(player[y].trousers) step[1]++;
				if(player[x].home!=player[y].home) step[1]++;
				if(player[x].knives[1]<=0) step[1]++;
			}
			else{
				step[i]=up(ac+player[y].khp,i-1);
				if(player[x].home!=player[y].home) step[i]++;
				if(player[x].knives[i]<=0) step[i]++;
			}
		}
	}
	else{
		for(int i=1;i<=3;i++){
			if(i==1){
				step[1]=1;
				if(player[y].trousers) step[1]++;
				if(player[x].home!=player[y].home) step[1]++;
				if(player[x].knives[1]<=0) step[1]++;
			}
			else{
				step[i]=up(ac+player[y].khp,i-1);
				if(player[x].home!=player[y].home) step[i]++;
				if(player[x].knives[i]<=0) step[i]++;
			}
		}
		for(int i=1;i<=player[x].gun_num;i++){
			int step112,step113;
			step112=step113=100;
			int damage=max(player[x].gun[i].long_gun,player[x].gun[i].short_gun);
			if(player[x].gun[i].short_gun>0){
				step112=up(ac+1,damage);
			}
			if(player[x].gun[i].long_gun>0){
				step113=up(ac+1,damage)*2;
				if(player[x].gun[i].toward==y) step113--;
			}
			if(!player[x].gun[i].bullet){
				step112++;step113++;
			}
			if(player[x].home!=player[y].home) step112++;
			step[player[x].gun[i].id]=min(step[player[x].gun[i].id],min(step112,step113));	
		}
		if(player[x].gun_num<3){//计算持有武器 
			for(int i=4;i<=player[x].level;i++){
				if(step[i]<100) continue;
				step[i]=1;
				int step112,step113;
				step112=step113=100;
				int k=i-3;
				int damage=max(gun0[k].long_gun,gun0[k].short_gun);
				if(gun0[k].short_gun>0){
					step112=up(ac+1,damage);
				}
				if(gun0[k].long_gun>0){
					step113=up(ac+1,damage)*2;
					
				}
				step112++;step113++;//装弹 
				if(player[x].home!=player[y].home) step112++;
				step[i]=min(step112,step113)+1;	
			} 
		}
	}
	for(int i=1;i<=player[x].level;i++){
		step[0]=min(step[0],step[i]);
	}
	return step[0];
}
///
int search_kill(int x){//AI3索敌 
	int the_kill=0,maxx=0,cntr=2;
	for(int i=1;i<=player_num;i++){
		if(i==x) continue;
		if(player[i].life<=0) continue;
		if(player[i].level>maxx){
			the_kill=i;
			maxx=player[i].level;
			cntr=2;
		}
		else if(player[i].level==maxx){
			if(rand()%cntr==0){
				the_kill=i;
			}
			cntr++;
		}
	}
	return the_kill;
}
int AI3_step(int y){//选取步数最少的武器，返回掩码 
	int x=the_move;
	if(player[x].level<=2){
		return 11;
	}
	else if(player[x].level>=3){
		int ac=0,step=100,damage=player[x].level-1;
		for(int i=1;i<=player[y].gun_num;i++){
			ac+=player[y].gun[i].AC;
		}
		step=up(ac+player[y].khp,damage); 
		if(player[x].home!=player[y].home) step++;
		 
		int step111=100;
		if(player[x].knives[1]){
			step111=1;
			if(player[x].home!=player[y].home) step111++;
			if(player[y].trousers) step111++;
		} 
		if(step111>step&&player[x].knives[3]>0){
			return 13;
		}
		else{
			return 11;
		}
	}
}
void AI3_move(int k,int y){//k武器攻击y 
	int x=the_move;
	
	if(k/10==1){//刀 
		k%=10;
		if(player[x].home!=player[y].home){
			m_struct=1;
			m_2=player[y].home;
			//AI1_move();
			player_move();
			return;
		}
		m_struct=5;
		m_2=10+k;
		m_3=y;
		//AI1_move();
		player_move();
		return;
	}
} 
///
void AI3(){//随机锁定除自己以外的最高等级，直到对方死亡 
	int x=the_move;
	if(player[player[x].kill].life<=0) player[x].kill=0;
	if(player[x].kill<=0) player[x].kill=search_kill(x);
	printf("当前索敌:%d\n",player[x].kill);//debug
	if(player[x].level<=2){
		if(!player[x].knives[1]){
			m_struct=2;
			m_2=1;
			player_move();
			return;
		}
	}
	else if(player[x].level>=3){
		if(!player[x].knives[3]&&player[player[x].kill].level<=5&&check3(x,player[x].kill,0)>1){
			m_struct=2;
			m_2=3;
			player_move();
			return;
		}
		if(!player[x].knives[1]&&check3(x,player[x].kill,0)>1){
			m_struct=2;
			m_2=1;
			player_move();
			return;
		}
	}
	if(player[x].level<=3){
		int tmp1=0;
		tmp1=check3(x,player[x].kill,0);
		if(tmp1>=4){//转为小刀击杀 
			m_struct=2;
			m_2=1;
			//AI1_move();
			player_move();
			return;
		}
		AI3_move(AI3_step(player[x].kill),player[x].kill);
		return;
	}
	else{
		int tmp1=0,tmp2=0;
		tmp1=check3(x,player[x].kill,0);
		tmp2=check3(player[x].kill,x,0);
		int y=player[x].kill;
		if(tmp1<=2){
			AI3_move(AI3_step(player[x].kill),player[x].kill);
			return;
		}
		else if(tmp2<=2){
			if(player[x].trousers<0&&player[y].knives[1]){
				m_struct=1;
				m_2=0;
				player_move();
				return;
			}
			if(player[x].gun_num<=3){
				int ac=0;
				for(int i=1;i<=player[x].gun_num;i++){
					ac+=player[x].gun[i].AC; 
				}
				int tmp3=check3(player[x].kill,x,gun0[player[x].level-3].AC);
				if(tmp3>tmp2){
					m_struct=2;
					m_2=player[x].level;
					player_move();
					return;
				}
			}
			AI3_move(AI3_step(player[x].kill),player[x].kill);
			return;
		}
		else{
			AI3_move(AI3_step(player[x].kill),player[x].kill);
			return;
		}
	}
	
} 

////////

/////// 


void new_game(){
	printf("///////新游戏开始，请输入人数：");
	player_num=get_int(2,9);
	player[1]=players(player[1].id,player[1].name,1,1);
	//for(int i=2;i<=player_num;i++){///测试阶段没有超级人机 ！！！ （AI1补位，AI2晚于AI3晚于AI4，最大数量为2,2,1 
	//	player[i]=players(player[i].id,player[i].name,1,2);
	//}
	/*正式AI分布
	player[2]=players(player[2].id,player[2].name,1,5);
	player[3]=players(player[3].id,player[3].name,1,4);
	player[4]=players(player[4].id,player[4].name,1,3);
	player[5]=players(player[5].id,player[5].name,1,4);
	player[6]=players(player[6].id,player[6].name,1,3);
	player[7]=players(player[7].id,player[7].name,1,2);
	player[8]=players(player[8].id,player[8].name,1,2);
	player[9]=players(player[9].id,player[9].name,1,2);
	*/
	/*测试AI分布*/ 
	player[2]=players(player[2].id,player[2].name,1,3);
	player[3]=players(player[3].id,player[3].name,1,4);
	player[4]=players(player[4].id,player[4].name,1,3);
	//player[4]=players(player[4].id,player[4].name,1,rand()%2+2);
	player[5]=players(player[5].id,player[5].name,1,2);
	player[6]=players(player[6].id,player[6].name,1,2);
	player[7]=players(player[7].id,player[7].name,1,2);
	player[8]=players(player[8].id,player[8].name,1,2);
	player[9]=players(player[9].id,player[9].name,1,2);
	printf("^_^玩游戏要啸着玩^_^/汪汪\n");
	return;
}

void GAME(){
	int a,b,c,win_id;
	while(1){//局数循环 
		printf("======新的一局游戏开始了======\n");
		while(1){//行动循环
			the_move=who_move();//测试时注释
			//测试指令代码
			//the_move=1;
			//test_move();
			//// 
			cout<<the_move<<"号玩家:"<<player[the_move].name<<"行动"<<endl; 
			if(player[the_move].player_class==1){
				user_move(1);
			//	test_move();
			}
			else if(player[the_move].player_class==2){
				AI1();//AI_stop();//调试禁用 
			}
			else if(player[the_move].player_class==3){
				AI2();//AI_stop();//调试禁用 
			}
			else if(player[the_move].player_class==4){
				AI3();//AI_stop();
			} 
			else{
				
			}
			if(life()){
				win_id=winner();
				for(int i=1;i<=player_num;i++){ 
					player[i]=players(player[i].id,player[i].name,player[i].level,player[i].player_class);
				}
				player[win_id]=players(player[win_id].id,player[win_id].name,
							player[win_id].level+1,player[win_id].player_class);
				lowest(win_id);//低保 
				break;
			}
		}
		if(player[win_id].level>9){
			printf("游戏结束，获胜者：");cout<<win_id<<"号玩家"<<player[win_id].name<<endl;
			break; 
		}
	} 
	return;
	
}
int main(){
	//freopen("small_knives测试指令.txt","r",stdin);
	//freopen("output.txt","w",stdout); 
	srand(time(0));
	int i,j,k;
	gun0[1]=guns(4,1,0,0,-1,1);//甲1远1 
	gun0[2]=guns(5,1,0,-1,1,-1);//甲1近1 
	gun0[3]=guns(6,2,0,0,2,2);//甲2近2远2 
	gun0[4]=guns(7,3,0,0,-1,3);//甲3远3 
	gun0[5]=guns(8,3,0,-1,3,-1);//甲3近3 
	gun0[6]=guns(9,3,0,0,3,3);//甲3近3远3 
	start();
	//freopen("small_knives.txt","r",stdin);
	while(1){
		new_game();
		GAME();
	//	break;//测试用代码 
	}
	return 0;
}
