
#include <stdio.h>

class Ticket
{
private:
  static const unsigned int OP_ADD = 1;
  static const unsigned int OP_SUB = 2;
  static const unsigned int OP_MUL = 3;
  static const unsigned int OP_DIV = 4;

  unsigned char digit[6];
  unsigned char op[6];
  unsigned char seq[6];
public:
  int init(const char* number)
  {
    int i = 0;

    while(i < 6 && number[i] != '\0')
    {
      if(!('0' <= number[i] && number[i] <= '9')) return 1;
      digit[i] = (unsigned char)(number[i] - '0');
      op[i] = seq[i] = 0;
      i++;
    }

    if(i != 6) return 1;

    return 0;
  }

public:
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

int main(int argn, char** argc)
{
  if(argn != 2)
  {
    printf("usage: %s <number>\n", argc[0]);
    return 1;
  }

  Ticket ticket;
  if(!ticket.init(argc[1]))
  {
    printf("error: wront number format\n");
    return 1;
  }

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
