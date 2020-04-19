#include <algorithm>
#include <iostream>
#include <sstream>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <string>
#include <vector>
using namespace std;

//有向图中找出所有的环，重复的环只保留字典序最小的序列：比如abcda,bcdab是同一个环，只保留abcda
/*
set<unsigned>s;是
set<set<unsigned> >set_circle_element;
比如发现有一个环是abcda，set_circle_element就记录了{a,b,c,d}。当发现bcdab这个环时，
因为set_circle_element中已经有 {a,b,c,d}了，就不考虑bcdab这个环了。
 vector<vector<unsigned> >CIRCLE;用来存储存储字典序最小的环
*/
vector<unsigned> expend;
vector<unsigned> circle;  //记录每次的序列，是最小字典序的环就加入到
                          // vector<vector<unsigned> >CIRCLE中
vector<vector<unsigned>> EXPAND;
vector<vector<unsigned>> CIRCLE; // 对circle排序后输出
map<unsigned, vector<unsigned>> mp;  //链式图
set<unsigned> s;  // 用来记录哪些元素构成环 （set具有不重复性）
set<set<unsigned>> set_circle_element;  // 用来存储每个环的元素,当有一个新环被发现，就被加入到里面。同时这保证了相同环的最小字典序.

/*
比较函数
vector元素小的排前面
要是两个vector一样大，就选字典序小的排前面
*/
int cmp(vector<unsigned> a, vector<unsigned> b) {
  if (a.size() != b.size())
    return a.size() < b.size();
  else {
    vector<unsigned>::iterator it1, it2;
    for (it1 = a.begin(), it2 = b.begin(); it1 != a.end(); it1++, it2++)
      if ((*it1) != (*it2))
        return (*it1) < (*it2);
      else
        continue;
    return 1;
  }
}

//打印序列
void show() {
  vector<unsigned>::iterator it2;
  for (it2 = circle.begin(); it2 != circle.end(); it2++) {
    cout << (*it2) << " ";
  }
  cout << endl;
}

void dfs(unsigned start) {
  //递归终点0 :序列的长度大于7了，必须return
  if (circle.size() > 7) return;

  //递归终点1
  // s发现有重复元素说明出现了环，并且判断这个新字符是否和circle序列第一个元素相等
  if (s.find(start) != s.end() && start == *circle.begin()) {
    // 长度小于3，不满足条件
    if (s.size() < 3) {
      return;
    }

    /*
     判断s在set_circle_element中是否出现过，
     如果出现过，说明此时的circle不是最小字典序，就不用添加了，否则添加
    */
    if (set_circle_element.find(s) == set_circle_element.end()) {
      set_circle_element.insert(s);  //添加新的元素组合
      CIRCLE.push_back(circle);      //添加一个新的字典序最小的环
    }
    return;
  }

  //递归终点2
  /*
  如果这个新的元素start在circle中出现过
  说明此时start在circle的中间且不是第一个（因为递归终点1已经判断过了），
  要是再加入这个元素，会在circle序列中产生子环（abcdc），这是不允许的，所以return
  */
  vector<unsigned>::iterator ret;
  ret = find(circle.begin(), circle.end(), start);
  if (ret != circle.end()) return;

  //递归终点3 :序列的长度都大于结点个数了，必须return
  if (circle.size() > mp.size()) return;

  //这个start不符合上面三个递归终点，将其加入circle序列和s集合
  s.insert(start);          // s加入start
  circle.push_back(start);  // circle加入start
  // unsigned next;
  //用链式图（变量名为mp）索引为start的vector数组元素依次作为序列的下一个元素
  for (auto it = mp[start].begin(); it != mp[start].end(); it++) {
    // next = (*it);
    dfs(*it);
  }
  //这两条语句是为了恢复现场，恢复成没加入start的情形
  circle.pop_back();
  s.erase(start);
}

int main() {
  freopen("1.txt", "r", stdin);
  string input;

  vector<unsigned> buf(3);
  while (getline(cin, input)) {
    stringstream ss(input);
    string str;
    for (int i = 0; getline(ss, str, ','); ++i) {
      stringstream num(str);
      num >> buf[i]; 
    }
    mp[buf[0]].push_back(buf[1]);

  }
  
  //输出链式图
  // cout << "输出链式图" << endl;
  // auto it = mp.begin();
  // for (; it != mp.end(); it++) {
  //   unsigned start = it->first;

  //   cout << start << ":";
  //   // vector<unsigned>::iterator it2;
  //   for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
  //     cout << (*it2) << " ";
  //   }
  //   cout << endl;
  // }

  /*
      遍历链式图的每个结点，并用每个结点连接的元素作为
  */
  for (auto it = mp.begin(); it != mp.end(); it++)  //遍历链式图的每个结点
  {
    unsigned start = it->first;  //获取第一个结点
    for (auto it2 = it->second.begin(); it2 != it->second.end();
         it2++)  //并用每个结点连接的元素（vector）作为下一个元素
    {
      //每次换新元素时都要重新把start加入，因为dfs之后会把s和circle清空
      circle.push_back(start);
      s.insert(start);

      dfs(*it2);  //（*it2就是vector数组中的每个元素）开始递归。。。

      //恢复现场，为下一个vector中的元素充当circle序列中start之后的元素做准备
      s.clear();
      circle.clear();
    }
    //恢复现场，感觉这两句多余，但是为了保险，还是清除一下吧
    // s.clear();
    // circle.clear();
  }

  cout << "打印所有环" << endl;
  cout << CIRCLE.size() << endl;
  sort(CIRCLE.begin(), CIRCLE.end(), cmp);  //给 CIRCLE排序

  //打印 CIRCLE，每个人的打印方式不一样，自己修改
  vector<vector<unsigned> >::iterator it1;
  vector<unsigned>::iterator it2;
  for (it1 = CIRCLE.begin(); it1 != CIRCLE.end(); it1++) {
    auto end = (*it1).end() - 1;
    for (it2 = (*it1).begin(); it2 != end; it2++) {
      cout << (*it2) << ",";
    }
    cout << *it2;
    cout <<endl;
  }

  return 0;
}