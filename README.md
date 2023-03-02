# Low_Latency_order_book

This Repository contains an experimental low latency order_book
it uses a std::map to store bids and asks. Due to generallity of the task an std::map is used
in reality or production a more cache friendly data structure should be prefarred to reduce cache misses
that will improve performace


additions --O(1) but O(logn)on first order in level

modify--O(logn)

cancels are O(logn)

futhur modifications will be made in v2 of this order book to push tor O(1)
operations
