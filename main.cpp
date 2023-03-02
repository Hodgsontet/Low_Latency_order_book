#include<iostream>
#include <bits/stdc++.h>
#include "order_book_ds.h"

void print_tuple(std::tuple<float,int,std::string,bool> & tp)
{
    printf("firm: %d  price : %.3f  symbol: %s \n", std::get<1>(tp),std::get<0>(tp),std::get<2>(tp).c_str() );
}
int main()
{
    
    order_book ob1("aapl");
    std::cout<<ob1.symbol_name<<'\n';
    ob1.on_new_order({21,2001,"aapl",0}); //buy
    
     ob1.on_new_order({22.00,2014,"aapl",1}); //sell
     ob1.modify(2001,21.999999);
     

   
    for(auto &v : ob1.order_pointer_hash)
    {
        print_tuple(*(v.second) );
    }
    printf("********* SELL SIDE *******\n");
    for(auto &v : ob1.sell_side_map)
    {
        printf("price level : %.3f  \n",v.first);
        for(auto &n : v.second)
          {
               
           print_tuple(n );
           
         }
         printf("\n");
    }
    return 0;
}