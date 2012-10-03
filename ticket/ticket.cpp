#include <fstream.h>
ofstream fs1("f1.txt");


class chislo
{
  public:
  short znachenie1[6];
  float znachenie2[6];
  char operaciya[6];
  short skobka[6][2];

  chislo(short zn1, short zn2, short zn3, short zn4, short zn5, short zn6)
  {
    znachenie1[0] = zn1;
    znachenie1[1] = zn2;
    znachenie1[2] = zn3;
    znachenie1[3] = zn4;
    znachenie1[4] = zn5;
    znachenie1[5] = zn6;
    znachenie2[0] = zn1;
    znachenie2[1] = zn2;
    znachenie2[2] = zn3;
    znachenie2[3] = zn4;
    znachenie2[4] = zn5;
    znachenie2[5] = zn6;
    for(int i = 0;i < 6;i++)
    {
      skobka[i][0] = 0;
      skobka[i][1] = 0;
    }
  };

  void print()
  {
    for(int i = 0;i < 6;i++)
    {
      if(i != 0)cout <<" "<< operaciya[i] <<" ";
      for(int b = 0;b < skobka[i][0];b++)
      {
        cout <<"(";
      }
      cout << znachenie1[i];
      for(int b = 0;b < skobka[i][1];b++)
      {
        cout <<")";
      }
    }
  }
};


class list
{
  public:
  list *pred;
  list *sled;
  chislo *elem;
  static list *last;
  static list *begin;

  void add(chislo *a)
  {
    if(last == NULL)
    {
      begin = this;
      sled = NULL;
      pred = NULL;
      last = this;
      elem = a;
    }
    else
    {
      last->sled = this;
      pred = last;
      elem = a;
      sled = NULL;
      last = this;
    }
  }
  void sub()
  {
    
  }
};
list *list::last = NULL;
list *list::begin = NULL;


class mas
{
  public:
  short k;
  short *mm1;
  short *mm2;

  mas(int a)
  {
    mm1 = new short[a];
    mm2 = new short[a];
    k = a;
    for(int i = 0;i < a;i++)
    {
      mm1[i] = i;
      mm2[i] = i;
    }
  };
};


void print_if5(chislo ch)
{
  if(ch.znachenie2[0] == 100)
  {
    ch.print();
    cout <<" = "<< ch.znachenie2[0] <<"\n";
  }
}


mas def_mas(int t, mas m)
{
  mas y(m.k - 1);

  for(int i = 0;i < t;i++)
  { y.mm1[i] = m.mm1[i]; }

  for(int i = t;i < (m.k - 1);i++)
  { y.mm1[i] = m.mm1[i + 1]; }

  for(int i = 0;i < t - 1;i++)
  { y.mm2[i] = m.mm2[i]; }

  for(int i = (t - 1);i < (m.k - 1);i++)
  { y.mm2[i] = m.mm2[i + 1]; }

  return y;
}


void switch_op(int k, int l, int g, chislo ch, mas numb)
{
  void init_bb(int, mas, chislo);
  switch(g)
  {
    case 0:
    ch.znachenie2[numb.mm1[l - 1]] += ch.znachenie2[numb.mm1[l]];
    ch.operaciya[numb.mm1[l]] = '+';
    if(k == 2)
    { print_if5(ch); break; }
    ch.skobka[numb.mm1[l - 1]][0]++;
    ch.skobka[numb.mm2[l]][1]++;
    init_bb(k - 1, def_mas(l, numb), ch);
    break;

    case 1:
    ch.znachenie2[numb.mm1[l - 1]] -= ch.znachenie2[numb.mm1[l]];
    ch.operaciya[numb.mm1[l]] = '-';
    if(k == 2)
    { print_if5(ch); break; }
    ch.skobka[numb.mm1[l - 1]][0]++;
    ch.skobka[numb.mm2[l]][1]++;
    init_bb(k - 1, def_mas(l, numb), ch);
    break;

    case 2:
    ch.znachenie2[numb.mm1[l - 1]] *= ch.znachenie2[numb.mm1[l]];
    ch.operaciya[numb.mm1[l]] = '*';
    if(k == 2)
    { print_if5(ch); break; }
    init_bb(k - 1, def_mas(l, numb), ch);
    break;

    case 3:
    if(ch.znachenie2[numb.mm1[l]] == 0)
    { break; }
    ch.znachenie2[numb.mm1[l - 1]] /= ch.znachenie2[numb.mm1[l]];
    ch.operaciya[numb.mm1[l]] = '/';
    if(k == 2)
    { print_if5(ch); break; }
    init_bb(k - 1, def_mas(l, numb), ch);
    break;
  }
}


void init_bb(int k, mas numb, chislo ch)
{
  for(int l = 1;l < k;l++)
  {
    for(int g = 0;g < 4;g++)
    {
      switch_op(k, l, g, ch, numb);
    }
  }
}


int main()
{
  int b[6];
  for(int i = 0;i < 6;i++)
  {
    st:
    cout <<"b["<< i + 1 <<"] = ";cin >> b[i];
    if(b[i] < 0 || b[i] > 9)
    {
      cout <<"Неверный параметр!!!";
      return 1;
    }
  }
  mas m(6);
  chislo ch(b[0], b[1], b[2], b[3], b[4], b[5]);
  init_bb(6, m, ch);
  ch.znachenie1[0] = -ch.znachenie1[0];
  ch.znachenie2[0] = -ch.znachenie2[0];
  init_bb(6, m, ch);
  return 0;
}
