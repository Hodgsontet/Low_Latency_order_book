#include<iostream>
#include <map>
#include <unordered_map>
#include <list>
#include <cstring>
#define price 0
//order is a tuple{price, firmid ,symbol}
//o buy 1 sell
constexpr int firmid = 1;
constexpr int symbol = 2;
constexpr int side = 3;

using order = std::tuple<float,int,std::string,bool>;
class order_book{
public:
char symbol_name[6];

 order_book(char* sym): buy_side_map{}, sell_side_map{}, order_pointer_hash{}   { strcpy(symbol_name,sym); }
 

 void add_order(order && odr)
 {   //if firm already placed order for symbol return
    if(order_pointer_hash.count(std::get<firmid>(odr))) {return; }
    //check if price point exist
        //if exists get  
         auto &side_level_map = (std::get<side>(odr) ==1) ?sell_side_map :buy_side_map ;
        if (auto list_it = side_level_map.find(std::get<price>(odr)) ; list_it !=side_level_map.end() )
        { //(*list_it).second is now essentially a pointer to an std::list
            int fm_id = std::get<firmid>(odr);
            auto odr_ptr = (*list_it).second.insert( (*list_it).second.end(), std::move(odr));
            //move odr_ptr to hash
              order_pointer_hash.insert({fm_id,std::move(odr_ptr)});
            //trade();
            return;
        }else
        { // price level doesnt exist
            auto pr_level = std::get<price>(odr);
            auto fm_id = std::get<firmid>(odr);
            std::list<order> mylist;
            auto odr_ptr = mylist.insert(mylist.end(),std::move(odr));
            side_level_map.insert({pr_level, std::move(mylist)});
            order_pointer_hash.insert({fm_id,std::move(odr_ptr)});
            //trade
            return;
        }
 }

 void modify(int _frm_id,float _price)
 {
          if(auto it_pair = order_pointer_hash.find(_frm_id); it_pair !=order_pointer_hash.end()  ) {
            auto &side_level_map = (std::get<side>(*(it_pair->second)) ==1) ?sell_side_map :buy_side_map ; //if order is sell side
               //OrderListMap &side_level_map = ooit->second->order_side_ == eS_Buy ? buy_book_map_ : sell_book_map_;
                side_level_map[std::get<price>(*(it_pair->second))].erase(it_pair->second);
                //if new price level doesnt exist, create list at price
                if(auto list_it =  side_level_map.find(_price); list_it != side_level_map.end() )
                {
                auto temp_it =side_level_map.at(_price).insert(side_level_map.at(_price).end() ,{_price,std::get<1>(*(it_pair->second)), std::get<2>(*(it_pair->second)), std::get<3>(*(it_pair->second))} ); 
                order_pointer_hash[_frm_id] = std::move(temp_it);
                }else
                {
                    std::list<order> mylist;
                    auto odr_ptr = mylist.insert(mylist.end(),{_price,std::get<1>(*(it_pair->second)), std::get<2>(*(it_pair->second)), std::get<3>(*(it_pair->second))});
                    side_level_map.insert({_price, std::move(mylist)});
                    order_pointer_hash[_frm_id] = std::move(odr_ptr);
                }
            //----------------try execution ----------------------//
            if(!buy_side_map.empty() && !sell_side_map.empty())
            {
                 auto best_buy_itr = buy_side_map.end(); best_buy_itr--;
                auto best_sell_itr = sell_side_map.begin();
                if(std::get<price>(best_buy_itr->second.front()) >= std::get<price>(best_sell_itr->second.front()) )
                {
                         execute(best_buy_itr->second.front(),best_sell_itr->second.front());
                }

            }
           }
           //execute
 }
 
 void cancel(int _frm_id)
 {  if(auto it_pair = order_pointer_hash.find(_frm_id); it_pair !=order_pointer_hash.end()  ) //O(1)
      {
        auto &side_level_map = (std::get<side>(*(it_pair->second)) ==1) ?sell_side_map :buy_side_map ;
        if(auto level_list_ptr = side_level_map.find(std::get<price>(*(it_pair->second))); level_list_ptr->second.size() <=1) //O(logn)
        {
             side_level_map.erase(level_list_ptr); 
             order_pointer_hash.erase(_frm_id);
        }
        else{
           level_list_ptr->second.erase(it_pair->second); //a
           order_pointer_hash.erase(_frm_id);
        }
      }    
 }

 void on_new_order(order && odr)
 {
    if(order_pointer_hash.count(get<firmid>(odr))){ return;}
    // get side
    //auto best_deal = if there is a cross;
    auto  best_buy = buy_side_map.end();
    if(!buy_side_map.empty()){best_buy--;}   
     auto  best_deal_itr = std::get<side>(odr)==1 ?best_buy : sell_side_map.begin() ;
     //auto& best_order_deal = best_deal_itr->second.front(); //ref to order
      switch (std::get<side>(odr))
          {
            case 0 : // wer bid
               {if(!sell_side_map.empty() && std::get<price>(odr) >= std::get<price>(best_deal_itr->second.front()) )
                    {execute(std::move(odr),best_deal_itr->second.front() );}
                else{  add_order(std::move(odr)); }
                 break;                
               }
            case 1 : // wer an ask
               {if(!buy_side_map.empty() && std::get<price>(odr) <= std::get<price>(best_deal_itr->second.front()) )
                    {execute(std::move(odr),best_deal_itr->second.front() );}
                else{  add_order(std::move(odr)); }
                 break;                
               }
               default:
                 break;
                
          }
    //execute v1 
 }
 void execute(order && odr, order& odr_node_ptr )
 {
        //record trade
        std::cout<<"trade*************trade\n";

        cancel(std::get<firmid>(odr_node_ptr) );
 }
 void execute(order & odr1, order& odr2 )
 {
        //record trade
        std::cout<<"trade*************trade\n";
        cancel(std::get<firmid>(odr1) );
        cancel(std::get<firmid>(odr2) );
 }
 std::unordered_map<int,std::list<order>::iterator > order_pointer_hash;
 std::map<float, std::list<order>  > buy_side_map;
std::map<float, std::list<order>> sell_side_map;

private:   

};