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
string guns_name[11]={"","С��","�˵�","����","Сǹ","���ǹ","AK","��","ɢ��","����ǹ","WINNER"}; //ǹеid���������Ӧ 
// С�� +998244353 �˵� 1 ���� 2 
struct guns{
	int id;//ǹ�ı�� 
	int AC;//���׵������۳���Ϻ�ǹе���� 
	int bullet;//�ӵ���Ŀ��װ������Ϊ�����ӵ�
	int toward;//ǹ�ڳ���Զ��������ֵΪ0������������ֵΪ-1��ʾ����Զ��
	int short_gun;//����������ֵΪ�˺�ֵ��Զ��Ϊ-1������ֻ��AK�ͻ���ǹԶ������
	int long_gun;//Զ��������ֵΪ�˺�ֵ������Ϊ-1������ֻ��AK�ͻ���ǹԶ������
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
	gun0[1]=guns(4,1,0,0,-1,1);//��1Զ1 
	gun0[2]=guns(5,1,0,-1,1,-1);//��1��1 
	gun0[3]=guns(6,2,0,0,2,2);//��2��2Զ2 
	gun0[4]=guns(7,3,0,0,-1,3);//��3Զ3 
	gun0[5]=guns(8,3,0,-1,3,-1);//��3��3 
	gun0[6]=guns(9,3,0,0,3,3);//��3��3Զ3 
	*/
string user_name;
struct players{
	int id;   //���
	string name;//����
	int player_class;//����������ҺͲ�ͬ�����˻�,����Ϊ1,��ͨ�˻�Ϊ2���߼�Ϊ3 
	int level;//���� 
	int knives[5];//1С��2�˵�3���������ڼ�¼��������û�����ޣ�
	guns gun[4];//���ڴ洢ǹе��������3��
				//����ֵ���ȿ۳�����е�����
	int gun_num;//��¼ǹе���� 
	int khp;//���е�������Ѫ��2
	int ghp;//����ǹ������Ѫ��1
	int life;//�ж����״̬
	bool trousers;//С�����п��ӣ���ֵ��Ϊ1 
	int home;//�ң���λ��Ϣ 
	int kill;//���У�AI2��AI3�� 
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
		name="Ĭ������";
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
	players(int a1,string a2,int a3,int a4){//����ÿ����Ϸ��ʼ�� 
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
//����ȫ������
void start();//����������ĳ�ʼ�� 
void show(int x);//չʾָ�������Ϣ��0Ϊ���� 
void show_gun(guns x);//show�ĸ������� 
//string get_order(int a);
bool life();//ֻʣһ�˷���true
int winner();//��ʤ��id 
void lowest(int win_id);//�ͱ�
bool player_move();//���ݲ����ж� 
void test_move();//����ģʽ������ӣ���Ҫ�ĵ��Ժ������޸ĺ��� 
int get_int();//��ȡһ����[l,r]��Χ�ڵķǸ�������0��l��r<1
void user_move(bool is_first,bool call_from_second);//��ȡ����ж�����
void user_move_second();//user_move()���Ӻ������������ѯ��ʱʹ��return
int who_move();//����˭�ж�������id 
void AI_stop();//AIִ�н�������ȴ��û�����س��Լ�������Ҫ�ĺ��ų���
void kkk();//���ų���1
void sc03();//���ų���2 
bool AI1_move();//���ݲ����ж� ��û�д�����ʾ
int AI1_random(int a1,int a2,int a3,int a4);//AI1�ж����͸��ʣ���Ȩ�أ����ɺܲ�
int AI1_random2(int x);//�ƶ����У������µ���1/3��������ϵ�2/3 
void AI1();//AI1�ж����� 
int up(int x,int y);//������ȡ��
int check2(int x,int y);//AI2ר�ã�x������������ɱy���貽��
int AI2_step(int y);//ѡȡ�������ٵ����������ز��� 
void AI2_move(int k,int y);//AI2ר�ã���k��������y���Զ�ִ����Ӧ���� 
void AI2();//AI2�ж������������߼��������ȣ������������ȣ�10%���� 
int check3(int x,int y,int z);//x��ɱyͨ�ü��㺯������������////z ���ڲ��令�׼��� 
int search_kill(int x);//AI3����  
int AI3_step(int y);//AI3ר�ã�ѡȡ�������ٵ����������ز���
void AI3_move(int k,int y);//AI3ר�ã�k��������y 
void AI3();//����������Լ��������ߵȼ���ֱ���Է����� 
void new_game();//һ���µ���Ϸ 
void GAME();//��Ϸ������
 
void start(){//����������ĳ�ʼ�� 
	printf("���մ�����");
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
///////��Ϸ����� ////////////////////
/*
string get_order(int a){
	string s;
	if(a==0){
		cin>>s;
	}
	if(a==1){
		printf("������ָ�");
		cin>>s;
	}
	if(a==2){
		printf("Debug\n");
	}
	
	return s;
	
}*/
int player_num;//����� 
void show_gun(guns x){//
	cout<<guns_name[x.id]<<"  ";
	printf("ʣ�໤�ף�%d  ",x.AC);
	if(x.bullet) printf("��װ��  ");
	else printf("δװ��  ");
	if(x.toward>0) printf("��׼�ˣ�%d\n",x.toward);
	else printf("\n");
	return;
}
void show(int x){//չʾָ�������Ϣ��0Ϊ���� 
	if(!x){
		for(int i=1;i<=player_num;i++){
			show(i);
		}
		for(int i=1;i<=player_num;i++){
			cout<<i<<"��:";
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
			printf("����%d",player[x].level);cout<<guns_name[player[x].level]<<"  ";
			printf("λ��%d\n",player[x].home);
			if(player[x].trousers) printf("���Ӵ���  ");
			else printf("���Ӳ�����  ");
			if(player[x].khp==1) printf("��Ѫ\n");
			else printf("��Ѫ\n");
			if(player[x].knives[1]) printf("С��%d��\n",player[x].knives[1]);
			if(player[x].knives[2]) printf("�˵�%d��\n",player[x].knives[2]);
			if(player[x].knives[3]) printf("����%d��\n",player[x].knives[3]);
			for(int i=1;i<=player[x].gun_num;i++){
				show_gun(player[x].gun[i]);
			} 
		}
		else{
			printf("������\n");
		}
	}
	return; 
}
bool life(){//ֻʣһ�˷���true 
	int a=0;
	for(int i=1;i<=player_num;i++){
		if(player[i].life) a++;
	}
	if(a==1) return true;
	return false;
}
int winner(){//��ʤ��id 
	for(int i=1;i<=player_num;i++){
		if(player[i].life) return i;
	}
	return 0;
} 
void lowest(int win_id){//�ͱ�
	cout<<"���"<<player[win_id].name<<"���غϻ�ʤ����ǰ�����е�������:";
	cout<<guns_name[player[win_id].level]<<endl;
	int minn=9,maxx=0;
	for(int i=1;i<=player_num;i++){
		minn=min(minn,player[i].level);
		maxx=max(maxx,player[i].level);
	}
	if(maxx>minn+3){
		minn=maxx-4;
		printf("��ǰ��߼�������Ϊ��");
		cout<<guns_name[maxx]<<endl;
		printf("��������Զ������������ڣ�");
		cout<<guns_name[minn+1]<<endl; 
		for(int i=1;i<=player_num;i++){
			player[i].level=max(player[i].level,minn+1); 
		}
	}
	return;
}
///�ж�����
int the_move;//�ж���id 
int m_struct;//�ж�����
int m_2;//��2���� 
int m_3;//��3����
/*�ж����ͣ�1�ƶ� 		��2�������ҵ�id
			2��ȡ����	��2�������������ͱ��
			3װ��		��2������ǹе���ͱ��(Ĭ����������е�ǹе��
			4��׼		��2������ǹеλ�ñ�ţ�123��			��3��������׼���� 
			5����		��2�������������ͱ��&123(2λ�����룩	��3��������������(�Զ�������Ӧ�ж���
			6����		��2�������������						��3����������������Ҳ�д��	
			7ʰȡ
			0show 
*/
// 
/// 
bool player_move(){//���ݲ����ж� 
	int x=the_move;
	if(m_struct==1){
		if(m_2>player_num){
			printf("������������'��'\n");
			return true;
		}
		else if(m_2<0){
			printf("������ȥ����תת��\n");
			return true;
		}
		else if(m_2==0){//��������-�����//
			if(player[x].trousers>0){
				printf("�����ſ��ӣ���������һ���µĴ���\n");
				return true;
			}
			else{
				cout<<x<<"�����"<<player[x].name<<"С��������������Լ��Ŀ���\n";
				player[x].trousers=1;
				return false; 
			}
		}
		else if(m_2==player[x].home){
			for(int i=1;i<=player_num;i++){//����������׼ 
				for(int j=1;j<=player[i].gun_num;j++){
					if(player[i].gun[j].toward==x){
						player[i].gun[j].toward=0;
					}
				}
			} 
			cout<<x<<"�����"<<player[x].name<<"���������е���׼\n";
			return false;
		}
		else{
			cout<<x<<"�����"<<player[x].name<<" �ƶ��� "<<m_2<<"�ŷ���\n";
			player[x].home=m_2;
			for(int i=1;i<=player[x].gun_num;i++){//�ƶ����Լ���׼ʧЧ 
				if(player[x].gun[i].toward>0){
					player[x].gun[i].toward=0;
				}
			} 
			for(int i=1;i<=player_num;i++){//����������׼ 
				for(int j=1;j<=player[i].gun_num;j++){
					if(player[i].gun[j].toward==x){
						player[i].gun[j].toward=0;
					}
				}
			} 
			return false;
		}
		printf("�ƶ���δ֪����\n");
		return true;
	}
	else if(m_struct==2){
		if(m_2>player[x].level){
			printf("����ȡ�ñ��Լ��ȼ����ߵ�����\n"); 
			return true;
		}
		else if(m_2>=1&&m_2<=3){
			player[x].knives[m_2]++;
			cout<<x<<"�����"<<player[x].name<<" ������ "<<guns_name[m_2]<<"\n";
			return false;
		}
		else if(m_2>=4&&m_2<=9){
			if(player[x].gun_num>=3){
				printf("ǹе�����ﵽ����\n");
				return true;
			}
			else{
				player[x].gun[++player[x].gun_num]=gun0[m_2-3];
				cout<<x<<"�����"<<player[x].name<<" ������ "<<guns_name[m_2]<<"\n";
				return false;
			}
		}
		else{
			printf("�Ƿ���ǹ/��\n");
			return true; 
		}
		printf("���е�δ֪����\n"); 
		return true;
	}
	else if(m_struct==3){
		for(int i=1;i<=player[x].gun_num;i++){
			if(player[x].gun[i].id==m_2&&player[x].gun[i].bullet<=0){
				cout<<x<<"�����"<<player[x].name<<" �Ե� "<<i<<"��ǹ-"<<guns_name[m_2]<<"-װ�����ӵ�\n";
				player[x].gun[i].bullet=32765;
				return false; 
			}
		} 
		printf("װ�������������ǹе�Ƿ���ڻ��Ѿ���װ��\n");
		return true;
	}
	else if(m_struct==4){
		if(!(m_3>=1&&m_3<=player_num)){
			printf("��ֹ����\n");
			return true; 
		} 
		if(!player[m_3].life){
			printf("��ֹ��ʬ\n");
			return true;
		}
		/*if(m_2>=1&&m_2<=3){
			printf("����û����׼\n");
			return true; 
		}
		if(m_2>9||m_2<=0){
			printf("�����ڸ��������\n");
			return true;
		}
		if(gun0[m_2].long_gun==-1){
			printf("�����������������׼\n");
			return true;
		}
		*/
		if(player[x].gun[m_2].toward>=0&&m_2<=player[x].gun_num&&m_2>0){
			cout<<x<<"�����"<<player[x].name<<" ���� "<<m_2<<"��ǹ-"<<guns_name[player[x].gun[m_2].id]<<"-��׼��";//�ֳ����кÿ� 
			cout<<m_3<<"�����"<<player[m_3].name<<"\n";
			player[x].gun[m_2].toward=m_3; 
			return false; 
		}
		printf("��׼�������������Ƿ��и�����,������������������׼\n");
		return true;
	}
	else if(m_struct==5){
		if(!player[m_3].life){
			printf("��ֹ��ʬ\n");
			return true;
		}
		if(!(m_3>=1&&m_3<=player_num)){
			printf("��ֹ����\n");
			return true; 
		}
		if(m_3==x){
			printf("��Ϸ����Ҫ�ʶ�Ŷ\n");//����ִ������ 
		}
		if(m_2%10==1&&m_2/10==1){
			m_2%=10;
			if(!player[x].knives[1]){
				cout<<"���ȳ���"<<guns_name[1]<<endl;
				return true;
			}
			if(player[x].home!=player[m_3].home){
				printf("��ս��������Ҫ����һ��Ŷ~\n");
				return true;
			}
			if(player[m_3].trousers>0){
				if(m_3==x){
					cout<<x<<"�����"<<player[x].name<<"�������Լ��Ŀ��ӣ�����ʾ����̫����\n";
					player[x].trousers=0;
				}
				else{
					cout<<player[x].name<<" ������ ";//�ֳ����кÿ� 
					cout<<m_3<<"�����"<<player[m_3].name<<" �Ŀ�����\n";
					player[m_3].trousers=0;
				}
				return false;
				
			}
			else{
				cout<<x<<"�����"<<player[x].name<<" ʹ��-"<<guns_name[m_2]<<"-��ɱ��";//�ֳ����кÿ� 
				cout<<m_3<<"�����"<<player[m_3].name<<"\n";
				player[m_3].life=0; 
				return false;
			}
			printf("С��bug\n");
			return true;
			
		}
		
		else if(m_2/10==1&&(m_2%10==2||m_2%10==3)){
			m_2%=10;
			int hurt=0;
			if(!player[x].knives[m_2]){
				cout<<"���ȳ���"<<guns_name[m_2]<<endl;
				return true; 
			}
			if(player[x].home!=player[m_3].home){
				printf("��ս��������Ҫ����һ��Ŷ~\n");
				return true;
			}
			hurt=m_2-1;
			cout<<x<<"�����"<<player[x].name<<" ʹ��-"<<guns_name[m_2]<<"-����";//�ֳ����кÿ� 
			cout<<m_3<<"�����"<<player[m_3].name<<"\n";
			
			for(int i=player[m_3].gun_num;i>=1;i--){
				if(hurt>=player[m_3].gun[i].AC){
					hurt-=player[m_3].gun[i].AC;
					player[m_3].gun[i].AC=0;
					cout<<m_3<<"�����"<<player[m_3].name<<"��"<<i<<"��ǹ-"<<guns_name[player[m_3].gun[i].id]<<"-������\n";
					player[m_3].gun_num--;
				}
				else{
					player[m_3].gun[i].AC-=hurt;
					hurt=0;
					break;
				}
				
			}
			cout<<m_3<<"�����"<<player[m_3].name<<"�ܵ�"<<hurt<<"�㵶��"<<"\n";
			player[m_3].khp-=hurt;
			if(player[m_3].khp<=0){
				cout<<x<<"�����"<<player[x].name<<" ʹ��-"<<guns_name[m_2]<<"-��ɱ��";//�ֳ����кÿ� 
				cout<<m_3<<"�����"<<player[m_3].name<<"\n";
				player[m_3].life=0; 
			} 
			return false;
			//printf("��bug\n");
			//return true;
		}
		else if(m_2/10==2){
			m_2%=10;
			if(!(m_2>=1&&m_2<=player[x].gun_num)){
				printf("����������\n");
				return true;
			}
			if(player[x].gun[m_2].bullet<=0){
				printf("ǹеδװ���ӵ�\n");
				return true;
			}
			//������Σ���ǹ��ǹ�ڳ�����׼��ʧЧ 
			if(player[x].gun[m_2].short_gun>0&&player[x].home==player[m_3].home){
				int hurt=player[x].gun[m_2].short_gun;
				int xxx=player[x].gun[m_2].id;
				cout<<x<<"�����"<<player[x].name<<" ʹ��-"<<guns_name[xxx]<<"-���̹���";//�ֳ����кÿ� 
				cout<<m_3<<"�����"<<player[m_3].name<<"\n";
				for(int i=player[m_3].gun_num;i>=1;i--){
					if(hurt>=player[m_3].gun[i].AC){
						hurt-=player[m_3].gun[i].AC;
						player[m_3].gun[i].AC=0;
						cout<<m_3<<"�����"<<player[m_3].name<<"��"<<i<<"��ǹ-"<<guns_name[player[m_3].gun[i].id]<<"-������\n";
						player[m_3].gun_num--;
					}
					else{
						player[m_3].gun[i].AC-=hurt;
						hurt=0;
						break;
					}
					
				}
				cout<<m_3<<"�����"<<player[m_3].name<<"�ܵ�"<<hurt<<"��ǹ��"<<"\n";
				player[m_3].ghp-=hurt;
				if(player[m_3].ghp<=0){
					cout<<x<<"�����"<<player[x].name<<" ʹ��-"<<guns_name[xxx]<<"-��ɱ��";//�ֳ����кÿ� 
					cout<<m_3<<"�����"<<player[m_3].name<<"\n";
					player[m_3].life=0; 
				} 
				if(player[x].gun[m_2].toward>0){
					player[x].gun[m_2].toward=0;
					printf("��ǹ��ǹ����׼��ʧЧ\n");
				}
				return false;
			}
			else if(player[x].gun[m_2].toward==m_3){
				int hurt=player[x].gun[m_2].long_gun;
				int xxx=player[x].gun[m_2].id;
				cout<<x<<"�����"<<player[x].name<<" ʹ��-"<<guns_name[xxx]<<"-Զ�̹���";//�ֳ����кÿ� 
				cout<<m_3<<"�����"<<player[m_3].name<<"\n";
				for(int i=player[m_3].gun_num;i>=1;i--){
					if(hurt>=player[m_3].gun[i].AC){
						hurt-=player[m_3].gun[i].AC;
						player[m_3].gun[i].AC=0;
						cout<<m_3<<"�����"<<player[m_3].name<<"��"<<i<<"��ǹ-"<<guns_name[player[m_3].gun[i].id]<<"-������\n";
						player[m_3].gun_num--;
					}
					else{
						player[m_3].gun[i].AC-=hurt;
						hurt=0;
						break;
					}
					
				}
				cout<<m_3<<"�����"<<player[m_3].name<<"�ܵ�"<<hurt<<"��ǹ��"<<"\n";
				player[m_3].ghp-=hurt;
				if(player[m_3].ghp<=0){
					cout<<x<<"�����"<<player[x].name<<" ʹ��-"<<guns_name[xxx]<<"-��ɱ��";//�ֳ����кÿ� 
					cout<<m_3<<"�����"<<player[m_3].name<<"\n";
					player[m_3].life=0; 
				} 
				if(player[x].gun[m_2].toward>0){
					player[x].gun[m_2].toward=0;
					printf("��ǹ��ǹ����׼��ʧЧ\n");
				}
				return false;
			}
			else{
				printf("Զ��δ��׼/����δ����һ��\n");
				return true;
			}
		}
		printf("������δ֪����\n"); 
		return true;
	}
	else if(m_struct==6){//���� 
		printf("�ù��ܻ�û�п��ţ���һ���ж�����\n");
		return true; 
	}
	else if(m_struct==7){//ʰȡ 
		printf("�ù��ܻ�û�п��ţ���һ���ж�����\n");
		return true;
	}
	else{
		m_struct=m_2=m_3=0;
		printf("�ж����Ͳ�������\n");
		return true; 
	}
	
}
void test_move(){//����ģʽ������� 
	printf("������ָ�");
	cin>>m_struct>>m_2>>m_3;
	if(m_struct==0){
		show(m_2);
	}
	else{
		player_move();
	}
}
////�û����� 
int get_int(int l,int r){//��ȡһ����[l,r]��Χ�ڵķǸ�������0��l��r<10
	string player_input;
	int in=-1;
	string back="return\0";
	do{
		cin>>player_input;
		if(player_input==back) return -1;
		if(player_input.length()>1)
		{
			printf("���벻�Ϸ������������룺");
			continue;
		}
		in=player_input[0]-'0';
		if(in<l||in>r) printf("���벻�Ϸ������������룺");
	}while(in<l||in>r);
	getchar();
	return in;
}
int move_stop;//��ֹ��return����ε���user_move
void user_move(bool is_first,bool call_from_second=0){//��ȡ����ж����� 
	if(!call_from_second){//����second�����������˲���
		if(is_first) printf("�� %s �ж��ˣ��������ж���ʽǰ����������\n",player[the_move].name.c_str());
		printf("0.չʾ�����Ϣ�����ж��������������ж����ᣩ\n");
		printf("1.�ƶ�/����/���Ͽ���\n");
		printf("2.��ȡ����\n");
		printf("3.װ���ӵ�\n");
		printf("4.��׼\n");
		printf("5.����\n");
	//	printf("6.����/��ʰ����\n");
		int player_input;
		player_input=get_int(0,5);
		m_struct=player_input;
		move_stop=0;//��ֹ��ε��� 
		user_move_second();
	}
	if(move_stop) return;
	int l,r;
	switch(m_struct){//���ݵ�һ������������ֵ�����ѯ?
		//0�Ѿ�д����
		case 4:{
			printf("���ֹ���%d����ң�������ʹ��\"%s\"������׼����������׼����ı�ţ�\n",player_num,guns_name[player[the_move].gun[m_2].id].c_str());
			l=1,r=player_num;
		}break;
		case 5:{
			int cnt=0,kind_weapon=0;//kind����������
			for(int i=1;i<=3;++i){//�����ں����䴫�����ˣ�������һ����������
				if(player[the_move].knives[i]){
					++cnt;
					if(cnt==m_2){//˵���û�ѡ����ǵ���
						m_2=10;//m_2�ĵ�һλΪ1��ʾ����Ϊ2��ʾǹ
						m_2+=i;
						kind_weapon=1;
					}
				}
			}
			if(kind_weapon==0){//������ѡ��Ϊǹ��������ֱ���������
				kind_weapon=2;
				m_2-=cnt;
				m_2+=20;//д������2022��7��21��21:34:24
				printf("���ֹ���%d����ң�������ʹ��\"%s\"���й����������빥������ı�ţ�\n",player_num,guns_name[player[the_move].gun[m_2%10].id].c_str());
			}
			else printf("���ֹ���%d����ң�������ʹ��\"%s\"���й����������빥������ı�ţ�\n",player_num,guns_name[m_2%10].c_str());
			l=1,r=player_num;
		}break;
		default:{//123����ֱ��Ϊplayer_move()������
			bool judge=player_move();//�ж��ж��Ƿ�Ϸ������Ϸ�����
			if(judge){
				user_move(0);
				return;
			}return;//������user_move_second���Ѿ�����
		}break;
	}
	printf("��������\"return\"�����ϼ�\n");
	int input_third;
	input_third=get_int(l,r);//��õ�������
	if(input_third==-1){//return
		user_move_second();
		return;
	}
	m_3=input_third;
	bool judge=player_move();//�ж��ж��Ƿ�Ϸ������Ϸ�����
	if(judge){
		user_move(0);
		return;
	}
	return;
}
void user_move_second(){//user_move()���Ӻ������������ѯ��ʱʹ��return
	int l,r;//��ʾ����ѯ������ֵ�ķ�Χ
	int guns_kind[10];//��case3��bz���Ƿ�ӵ��idΪi������
	switch(m_struct){//���������ѡ��������ѯ��
		case 0:{
			printf("���ֹ���%d����ң���������Ҫչʾ��Ϣ����ұ�ţ���������\"0\"���鿴ȫ������\n",player_num);
			l=0,r=player_num;
		}break;
		case 1:{
			printf("���ֹ���%d����ң���������Ҫ�ƶ����ķ����ţ�\n",player_num);
			printf("����ѡ���ƶ����������ڵ�λ�ã�����Ϊ���ܵ�����������ҵ���׼\n");
			printf("����������\"0\"������Ϊ���Ͽ���\n");
			l=0,r=player_num;
		}break;
		case 2:{
			printf("������%d�������Ի�õ������У�\n",player[the_move].level);
			for(int i=1;i<=player[the_move].level;++i) printf("%d.%s\n",i,guns_name[i].c_str());
			l=1,r=player[the_move].level;
		}break;
		case 3:{
			int t=player[the_move].gun_num;
			if(!t){
				printf("������û����װ����ǹе���������޷�װ���ӵ�����������ѡ�����......\n");
				Sleep(100);
				user_move(0);
				move_stop=1;//��ֹ��Ϊreturn����ε��ú���player_move 
				return;
			}
			printf("�������������������������ѡ��һ�ֽ���װ�ֻ��װ����������������õ�һ������\n");
			//int guns_kind[10];//bz���Ƿ�ӵ��idΪi������
			//��Ϊ��������ã��Ƶ�switch����
			memset(guns_kind,0,sizeof(guns_kind));
			for(int i=1;i<=t;++i) guns_kind[player[the_move].gun[i].id]=1;
			int cnt=0;
			for(int i=1;i<=9;++i){//��ӡ�����б�
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
				printf("������û����װ����ǹе���������޷���׼����������ѡ�����......\n");
				Sleep(100);
				user_move(0);
				move_stop=1;//��ֹ��Ϊreturn����ε��ú���player_move 
				return;
			}
			printf("������������ǹе����������ѡ��һ��������׼��\n");//��ӡ�����б�
			for(int i=1;i<=t;++i){
				printf("%d.%s\n",i,guns_name[player[the_move].gun[i].id].c_str());
			}
			l=1,r=t;
		}break;
		case 5:{
			int cnt=0;
			int t=player[the_move].gun_num;
			if(player[the_move].knives[1]==0&&player[the_move].knives[2]==0&&player[the_move].knives[3]==0&&t==0){
				printf("������û���κ��������޷���������������ѡ�����......\n");
				Sleep(100);
				user_move(0);
				move_stop=1;//��ֹ��Ϊreturn����ε��ú���player_move 
				return;
			}
			printf("��������������������ѡ��һ�����й�����\n");//��ӡ�����б�
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
		default:{//�ڴ����ʱ��������return
			printf("�޷�����return��Ŷ��\n");
			user_move(1);
			move_stop=1;//��ֹ��Ϊreturn����ε��ú���player_move 
			return;
		}break;
	}
	printf("��������\"return\"�����ϼ�\n");
	int input_second;
	input_second=get_int(l,r);//��õڶ�����
	if(input_second==-1){//return
		user_move(0);
		move_stop=1;//��ֹ��Ϊreturn����ε��ú���player_move 
		return;
	}
	if(m_struct==0){//չʾ������ѯ��
		show(input_second);
		user_move(0);
		move_stop=1;//��ֹ��Ϊreturn����ε��ú���player_move 
		return;
	}
	if(m_struct!=3) m_2=input_second;//���ڶ�����,װ��Ҫ���⴦�� 
	else{//���������ĳ����ǹ��id 
		int cnt=0;
		for(int i=1;i<=9;++i){//����ɨһ�飬����û������ǹ������
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
	move_stop=1;//��ֹ��Ϊreturn����ε��ú���player_move 
	return;
}
/////// 
int who_move(){//����˭�ж�������id 
	int mm=rand()%(player_num*4)/4+1;
	while(!player[mm].life){
		mm=rand()%(player_num*4)/4+1;
	}
	return mm;
}
///////AI���////////////////////////
void AI_stop(){//AIִ�н�������ȴ��û�����س��Լ���
	printf("==��ȷ���˻���ִ�в��裬���س�ʹ��Ϸ��������==");
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
void kkk(){//���ų���1,չʾ���� 
	for(int i=1;i<=player_num;i++){
		if(player[i].player_class==3||player[i].player_class==4){
			if(player[player[i].kill].life>=0&&player[i].kill){
				cout<<i<<"������˻����"<<player[i].player_class-1<<"������"<<player[i].kill<<endl;
			}
		}
	} 
}
void sc03(){//���ų���2���޸Ĳ��� 
	int xxx,x1,x2,x3;
	cin>>xxx;
	if(xxx==0){
		return;
	} 
	else if(xxx==1){//���ж��޸ĵĲ���
		cin>>x1;
		the_move=x1; 
		test_move();
	}
	else if(xxx==2){//level
		cin>>x1>>x2;
		player[x1].level=x2;
		printf("%d����Ҽ������Ϊ%d\n",x1,x2);
	}
	else if(xxx==3){//���˻ָ� 
		cin>>x1;
		player[x1].khp=2;
		printf("%d�������Ȭ��\n",x1);
	}
	else if(xxx==4){//���� 
		cin>>x1;
		player[x1].life=1;
		printf("%d������Ѹ���\n",x1);
	}
	else if(xxx==5){//�޸��˻���� 
		cin>>x1>>x2;
		player[x1].player_class=x2+1;
		printf("%d������˻�������Ϊ%d\n",x1,x2);
	}
	else if(xxx==6){//�޸����� 
		cin>>x1>>x2;
		player[x1].kill=x2;
		printf("%d������˻����и�Ϊ%d\n",x1,x2); 
	}
	printf("==��ȷ���޸Ĳ��裬������\"=\"ʹ��Ϸ��������==");
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
///Ԥ�� 

/// 1
bool AI1_move(){//���ݲ����ж� 
	int x=the_move;
	if(m_struct==1){
		if(m_2>player_num){
		//	printf("������������'��'\n");
			return true;
		}
		else if(m_2<0){
		//	printf("������ȥ����תת��\n");
			return true;
		}
		else if(m_2==0){//��������-�����//
			if(player[x].trousers>0){
		//		printf("�����ſ��ӣ���������һ���µĴ���\n");
				return true;
			}
			else{
				cout<<x<<"�����"<<player[x].name<<"С��������������Լ��Ŀ���\n";
				player[x].trousers=1;
				return false; 
			}
		}
		else if(m_2==player[x].home){
			for(int i=1;i<=player_num;i++){//����������׼ 
				for(int j=1;j<=player[i].gun_num;j++){
					if(player[i].gun[j].toward==x){
						player[i].gun[j].toward=0;
					}
				}
			} 
			cout<<x<<"�����"<<player[x].name<<"���������е���׼\n";
			return false;
		}
		else{
			cout<<x<<"�����"<<player[x].name<<" �ƶ��� "<<m_2<<"�ŷ���\n";
			player[x].home=m_2;
			for(int i=1;i<=player[x].gun_num;i++){//�ƶ����Լ���׼ʧЧ 
				if(player[x].gun[i].toward>0){
					player[x].gun[i].toward=0;
				}
			} 
			for(int i=1;i<=player_num;i++){//����������׼ 
				for(int j=1;j<=player[i].gun_num;j++){
					if(player[i].gun[j].toward==x){
						player[i].gun[j].toward=0;
					}
				}
			} 
			return false;
		}
	//	printf("�ƶ���δ֪����\n");
		return true;
	}
	else if(m_struct==2){
		if(m_2>player[x].level){
	//		printf("����ȡ�ñ��Լ��ȼ����ߵ�����\n"); 
			return true;
		}
		else if(m_2>=1&&m_2<=3){
			if(player[x].knives[m_2]) return true;//��ֹAI1�ظ�������-�� 
			player[x].knives[m_2]++;
			cout<<x<<"�����"<<player[x].name<<" ������ "<<guns_name[m_2]<<"\n";
			return false;
		}
		else if(m_2>=4&&m_2<=9){
			if(player[x].gun_num>=3){
	//			printf("ǹе�����ﵽ����\n");
				return true;
			}
			else{
				player[x].gun[++player[x].gun_num]=gun0[m_2-3];
				cout<<x<<"�����"<<player[x].name<<" ������ "<<guns_name[m_2]<<"\n";
				return false;
			}
		}
		else{
		//	printf("�Ƿ���ǹ/��\n");
			return true; 
		}
	//	printf("���е�δ֪����\n"); 
		return true;
	}
	else if(m_struct==3){
		for(int i=1;i<=player[x].gun_num;i++){
			if(player[x].gun[i].id==m_2&&player[x].gun[i].bullet<=0){
				cout<<x<<"�����"<<player[x].name<<" �Ե� "<<i<<"��ǹ-"<<guns_name[m_2]<<"-װ�����ӵ�\n";
				player[x].gun[i].bullet=32765;
				return false; 
			}
		} 
	//	printf("װ�������������ǹе�Ƿ���ڻ��Ѿ���װ��\n");
		return true;
	}
	else if(m_struct==4){
		if(!(m_3>=1&&m_3<=player_num)){
	//		printf("��ֹ����\n");
			return true; 
		} 
		if(!player[m_3].life){
	//		printf("��ֹ��ʬ\n");
			return true;
		}
		if(m_3==x){
		//	printf("��Ϸ����Ҫ�ʶ�Ŷ\n");//����ִ������ 
			return true;//��ֹAI1�Ե� 
		}
		/*if(m_2>=1&&m_2<=3){
			printf("����û����׼\n");
			return true; 
		}
		if(m_2>9||m_2<=0){
			printf("�����ڸ��������\n");
			return true;
		}
		if(gun0[m_2].long_gun==-1){
			printf("�����������������׼\n");
			return true;
		}
		*/
		if(player[x].gun[m_2].toward>=0&&m_2<=player[x].gun_num&&m_2>0){
			cout<<x<<"�����"<<player[x].name<<" ���� "<<m_2<<"��ǹ-"<<guns_name[player[x].gun[m_2].id]<<"-��׼��";//�ֳ����кÿ� 
			cout<<m_3<<"�����"<<player[m_3].name<<"\n";
			player[x].gun[m_2].toward=m_3; 
			return false; 
		}
	//	printf("��׼�������������Ƿ��и�����,������������������׼\n");
		return true;
	}
	else if(m_struct==5){
		if(!player[m_3].life){
	//		printf("��ֹ��ʬ\n");
			return true;
		}
		if(!(m_3>=1&&m_3<=player_num)){
	//		printf("��ֹ����\n");
			return true; 
		}
		if(m_3==x){
		//	printf("��Ϸ����Ҫ�ʶ�Ŷ\n");//����ִ������ 
			return true;//��ֹAI1�Ե� 
		}
		if(m_2%10==1&&m_2/10==1){
			m_2%=10;
			if(!player[x].knives[1]){
			//	cout<<"���ȳ���"<<guns_name[1]<<endl;
				return true;
			}
			if(player[x].home!=player[m_3].home){
			//	printf("��ս��������Ҫ����һ��Ŷ~\n");
				return true;
			}
			if(player[m_3].trousers>0){
				if(m_3==x){
					cout<<x<<"�����"<<player[x].name<<"�������Լ��Ŀ��ӣ�����ʾ����̫����\n";
					player[x].trousers=0;
				}
				else{
					cout<<player[x].name<<" ������ ";//�ֳ����кÿ� 
					cout<<m_3<<"�����"<<player[m_3].name<<" �Ŀ�����\n";
					player[m_3].trousers=0;
				}
				return false;
				
			}
			else{
				cout<<x<<"�����"<<player[x].name<<" ʹ��-"<<guns_name[m_2]<<"-��ɱ��";//�ֳ����кÿ� 
				cout<<m_3<<"�����"<<player[m_3].name<<"\n";
				player[m_3].life=0; 
				return false;
			}
			printf("С��bug\n");
			return true;
			
		}
		
		else if(m_2/10==1&&(m_2%10==2||m_2%10==3)){
			m_2%=10;
			int hurt=0;
			if(!player[x].knives[m_2]){
		//		cout<<"���ȳ���"<<guns_name[m_2]<<endl;
				return true; 
			}
			if(player[x].home!=player[m_3].home){
		//		printf("��ս��������Ҫ����һ��Ŷ~\n");
				return true;
			}
			hurt=m_2-1;
			cout<<x<<"�����"<<player[x].name<<" ʹ��-"<<guns_name[m_2]<<"-����";//�ֳ����кÿ� 
			cout<<m_3<<"�����"<<player[m_3].name<<"\n";
			
			for(int i=player[m_3].gun_num;i>=1;i--){
				if(hurt>=player[m_3].gun[i].AC){
					hurt-=player[m_3].gun[i].AC;
					player[m_3].gun[i].AC=0;
					cout<<m_3<<"�����"<<player[m_3].name<<"��"<<i<<"��ǹ-"<<guns_name[player[m_3].gun[i].id]<<"-������\n";
					player[m_3].gun_num--;
				}
				else{
					player[m_3].gun[i].AC-=hurt;
					hurt=0;
					break;
				}
				
			}
			cout<<m_3<<"�����"<<player[m_3].name<<"�ܵ�"<<hurt<<"�㵶��"<<"\n";
			player[m_3].khp-=hurt;
			if(player[m_3].khp<=0){
				cout<<x<<"�����"<<player[x].name<<" ʹ��-"<<guns_name[m_2]<<"-��ɱ��";//�ֳ����кÿ� 
				cout<<m_3<<"�����"<<player[m_3].name<<"\n";
				player[m_3].life=0; 
			} 
			return false;
			//printf("��bug\n");
			//return true;
		}
		else if(m_2/10==2){
			m_2%=10;
			if(!(m_2>=1&&m_2<=player[x].gun_num)){
		//		printf("����������\n");
				return true;
			}
			if(player[x].gun[m_2].bullet<=0){
		//		printf("ǹеδװ���ӵ�\n");
				return true;
			}
			//������Σ���ǹ��ǹ�ڳ�����׼��ʧЧ 
			if(player[x].gun[m_2].short_gun>0&&player[x].home==player[m_3].home){
				int hurt=player[x].gun[m_2].short_gun;
				int xxx=player[x].gun[m_2].id;
				cout<<x<<"�����"<<player[x].name<<" ʹ��-"<<guns_name[xxx]<<"-���̹���";//�ֳ����кÿ� 
				cout<<m_3<<"�����"<<player[m_3].name<<"\n";
				for(int i=player[m_3].gun_num;i>=1;i--){
					if(hurt>=player[m_3].gun[i].AC){
						hurt-=player[m_3].gun[i].AC;
						player[m_3].gun[i].AC=0;
						cout<<m_3<<"�����"<<player[m_3].name<<"��"<<i<<"��ǹ-"<<guns_name[player[m_3].gun[i].id]<<"-������\n";
						player[m_3].gun_num--;
					}
					else{
						player[m_3].gun[i].AC-=hurt;
						hurt=0;
						break;
					}
					
				}
				cout<<m_3<<"�����"<<player[m_3].name<<"�ܵ�"<<hurt<<"��ǹ��"<<"\n";
				player[m_3].ghp-=hurt;
				if(player[m_3].ghp<=0){
					cout<<x<<"�����"<<player[x].name<<" ʹ��-"<<guns_name[xxx]<<"-��ɱ��";//�ֳ����кÿ� 
					cout<<m_3<<"�����"<<player[m_3].name<<"\n";
					player[m_3].life=0; 
				} 
				if(player[x].gun[m_2].toward>0){
					player[x].gun[m_2].toward=0;
					printf("��ǹ��ǹ����׼��ʧЧ\n");
				}
				return false;
			}
			else if(player[x].gun[m_2].toward==m_3){
				int hurt=player[x].gun[m_2].long_gun;
				int xxx=player[x].gun[m_2].id;
				cout<<x<<"�����"<<player[x].name<<" ʹ��-"<<guns_name[xxx]<<"-Զ�̹���";//�ֳ����кÿ� 
				cout<<m_3<<"�����"<<player[m_3].name<<"\n";
				for(int i=player[m_3].gun_num;i>=1;i--){
					if(hurt>=player[m_3].gun[i].AC){
						hurt-=player[m_3].gun[i].AC;
						player[m_3].gun[i].AC=0;
						cout<<m_3<<"�����"<<player[m_3].name<<"��"<<i<<"��ǹ-"<<guns_name[player[m_3].gun[i].id]<<"-������\n";
						player[m_3].gun_num--;
					}
					else{
						player[m_3].gun[i].AC-=hurt;
						hurt=0;
						break;
					}
					
				}
				cout<<m_3<<"�����"<<player[m_3].name<<"�ܵ�"<<hurt<<"��ǹ��"<<"\n";
				player[m_3].ghp-=hurt;
				if(player[m_3].ghp<=0){
					cout<<x<<"�����"<<player[x].name<<" ʹ��-"<<guns_name[xxx]<<"-��ɱ��";//�ֳ����кÿ� 
					cout<<m_3<<"�����"<<player[m_3].name<<"\n";
					player[m_3].life=0; 
				} 
				if(player[x].gun[m_2].toward>0){
					player[x].gun[m_2].toward=0;
					printf("��ǹ��ǹ����׼��ʧЧ\n");
				}
				return false;
			}
			else{
		//		printf("Զ��δ��׼/����δ����һ��\n");
				return true;
			}
		}
		printf("������δ֪����\n"); 
		return true;
	}
	else if(m_struct==6){//���� 
		printf("�ù��ܻ�û�п��ţ���һ���ж�����\n");
		return true; 
	}
	else if(m_struct==7){//ʰȡ 
		printf("�ù��ܻ�û�п��ţ���һ���ж�����\n");
		return true;
	}
	else{
		m_struct=m_2=m_3=0;
		printf("�ж����Ͳ�������\n");
		return true; 
	}
	
}
int AI1_random(int a1,int a2,int a3,int a4){//AI1�ж����͸��� 
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
int AI1_random2(int x){//�ƶ����� 
	int y=rand()%(player_num+1);
	if(y==x){
		if(rand()%3) y=0;
	}
	return y;
}
void AI1(){//AI1�ж����� 
	//printf("�˻��ж��غ�\n");
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

int up(int x,int y){//������ȡ�� 
	if(x%y){
		return x/y+1;
	}
	return x/y;
}
int check2(int x,int y){//x������������ɱy���貽��
	if(player[x].level==1){
		//С�������ж� 
		int step111=100;
		if(player[x].knives[1]) step111=1;
		if(player[x].home!=player[y].home) step111++;
		if(player[y].trousers) step111++;
		return step111;
	}
	else if(player[x].level<=3){
		//���������ж� 
		int ac=0,step=100,damage=player[x].level-1;
		for(int i=1;i<=player[y].gun_num;i++){
			ac+=player[y].gun[i].AC;
		}
		step=up(ac+player[y].khp,damage); 
		if(player[x].home!=player[y].home) step++;
		//С�������ж� 
		int step111=100;
		if(player[x].knives[1]){
			step111=1;
			if(player[x].home!=player[y].home) step111++;
			if(player[y].trousers) step111++;
		} 
		
		
		return min(step,step111);
	}
	else if(player[x].level<=5){
		//�� 
		int ac=0,step=100,damage=2;
		for(int i=1;i<=player[y].gun_num;i++){
			ac+=player[y].gun[i].AC;
		}
		if(player[x].knives[3]){
			step=up(ac+player[y].khp,damage); 
			if(player[x].home!=player[y].home) step++; 
		}
		//С�� 
		int step111=100;
		if(player[x].knives[1]){
			step111=1;
			if(player[x].home!=player[y].home) step111++;
			if(player[y].trousers) step111++;
		}
		int step1=100;
		//ǹ 
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
int AI2_step(int y){//ѡȡ�������ٵ��������������� 
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
	
	if(k/10==1){//�� 
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
	else{//ǹ 
		k%=10;
		if(player[x].gun[k].bullet<=0){//װ���ж� 
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
void AI2(){//�����߼��������ȣ������������ȣ�10%���� 
	int x=the_move;
	if(player[player[x].kill].life<=0) player[x].kill=0;
	//���ȳ�������������������
	if(player[x].kill>0){//����������ɱ 
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
		else if(player[x].gun_num<1){//һ����ɱ����ǹ 
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
				m_2=4;//ֻҪСǹ�����ǹ�ۺ����治�� 
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
		//cout<<i<<"-����:"<<tmp2<<endl;//::debug
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
		//cout<<"enemy��"<<enemy<<endl; //::debug
		
	}
	//cout<<"����:"<<tmp1<<endl;//::debug
	//cout<<"������"<<enemy<<endl; //::debug
	if(tmp1>1){
		if(!player[x].trousers){//�������� 
			m_struct=1;m_2=0;
			//AI1_move();
			player_move();
			return;
		}
	}
	if(tmp1>=4){//תΪС����ɱ 
		m_struct=2;
		m_2=1;
		//AI1_move();
		player_move();
		return;
	}
	int the_gun=AI2_step(enemy);
	if(player[x].kill<=0&&rand()%10==0) player[x].kill=enemy;//���� 
	AI2_move(the_gun,enemy);
	return;
}
///3
///check3���丽������ 

int check3(int x,int y,int z){//x��ɱyͨ�ü��㺯������������
////z ���ڲ��令�׼���  
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
		if(player[x].gun_num<3){//����������� 
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
				step112++;step113++;//װ�� 
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
int search_kill(int x){//AI3���� 
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
int AI3_step(int y){//ѡȡ�������ٵ��������������� 
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
void AI3_move(int k,int y){//k��������y 
	int x=the_move;
	
	if(k/10==1){//�� 
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
void AI3(){//����������Լ��������ߵȼ���ֱ���Է����� 
	int x=the_move;
	if(player[player[x].kill].life<=0) player[x].kill=0;
	if(player[x].kill<=0) player[x].kill=search_kill(x);
	printf("��ǰ����:%d\n",player[x].kill);//debug
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
		if(tmp1>=4){//תΪС����ɱ 
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
	printf("///////����Ϸ��ʼ��������������");
	player_num=get_int(2,9);
	player[1]=players(player[1].id,player[1].name,1,1);
	//for(int i=2;i<=player_num;i++){///���Խ׶�û�г����˻� ������ ��AI1��λ��AI2����AI3����AI4���������Ϊ2,2,1 
	//	player[i]=players(player[i].id,player[i].name,1,2);
	//}
	/*��ʽAI�ֲ�
	player[2]=players(player[2].id,player[2].name,1,5);
	player[3]=players(player[3].id,player[3].name,1,4);
	player[4]=players(player[4].id,player[4].name,1,3);
	player[5]=players(player[5].id,player[5].name,1,4);
	player[6]=players(player[6].id,player[6].name,1,3);
	player[7]=players(player[7].id,player[7].name,1,2);
	player[8]=players(player[8].id,player[8].name,1,2);
	player[9]=players(player[9].id,player[9].name,1,2);
	*/
	/*����AI�ֲ�*/ 
	player[2]=players(player[2].id,player[2].name,1,3);
	player[3]=players(player[3].id,player[3].name,1,4);
	player[4]=players(player[4].id,player[4].name,1,3);
	//player[4]=players(player[4].id,player[4].name,1,rand()%2+2);
	player[5]=players(player[5].id,player[5].name,1,2);
	player[6]=players(player[6].id,player[6].name,1,2);
	player[7]=players(player[7].id,player[7].name,1,2);
	player[8]=players(player[8].id,player[8].name,1,2);
	player[9]=players(player[9].id,player[9].name,1,2);
	printf("^_^����ϷҪХ����^_^/����\n");
	return;
}

void GAME(){
	int a,b,c,win_id;
	while(1){//����ѭ�� 
		printf("======�µ�һ����Ϸ��ʼ��======\n");
		while(1){//�ж�ѭ��
			the_move=who_move();//����ʱע��
			//����ָ�����
			//the_move=1;
			//test_move();
			//// 
			cout<<the_move<<"�����:"<<player[the_move].name<<"�ж�"<<endl; 
			if(player[the_move].player_class==1){
				user_move(1);
			//	test_move();
			}
			else if(player[the_move].player_class==2){
				AI1();//AI_stop();//���Խ��� 
			}
			else if(player[the_move].player_class==3){
				AI2();//AI_stop();//���Խ��� 
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
				lowest(win_id);//�ͱ� 
				break;
			}
		}
		if(player[win_id].level>9){
			printf("��Ϸ��������ʤ�ߣ�");cout<<win_id<<"�����"<<player[win_id].name<<endl;
			break; 
		}
	} 
	return;
	
}
int main(){
	//freopen("small_knives����ָ��.txt","r",stdin);
	//freopen("output.txt","w",stdout); 
	srand(time(0));
	int i,j,k;
	gun0[1]=guns(4,1,0,0,-1,1);//��1Զ1 
	gun0[2]=guns(5,1,0,-1,1,-1);//��1��1 
	gun0[3]=guns(6,2,0,0,2,2);//��2��2Զ2 
	gun0[4]=guns(7,3,0,0,-1,3);//��3Զ3 
	gun0[5]=guns(8,3,0,-1,3,-1);//��3��3 
	gun0[6]=guns(9,3,0,0,3,3);//��3��3Զ3 
	start();
	//freopen("small_knives.txt","r",stdin);
	while(1){
		new_game();
		GAME();
	//	break;//�����ô��� 
	}
	return 0;
}
