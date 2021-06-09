# CSC360 Assignment4: A simulator of three page-replacement schemes for virtual-memory systems by Kanato Sato (V00948631)
## Scheme1: FIFO (First In First Out)
FIFO scheme, as the name implies, replaces the oldest page with the new one when page fault occurs.
If there is a free frame (also page fault), simply the new page occupies the frame.

My approach to implement FIFO sceme is as follows:
I created a new element named *int time* in the list *page_table_entry*
*int time* stores the line number of the page in the input txt file.
If a page found in a frame (ie, page fault didn't occur), *time* does not get updated.
*time* gets updated only when page fault occured.
Then find a frame with the smallest *time* (so oldest), which is the victim frame of FIFO scheme.

## Scheme2: LRU (Least Recently Used)
LRU scheme replaces the least recently used page with the new one when page fault occurs.
If there is a free frame (also page fault), simply the new page occupies the frame.

My approach to implement LRU sceme is as follows:
Similar to FIFO, I created a new element named *int lru_time* in the list *page_table_entry*.
*int lru_time* stores the most recent time the page in the frame used (when a page found in a frame, update *lru_time*).
If a page fault occured, *lru_time* is set the same value as *time*.
Then find a frame with the smallest *lru_time* (so least recently used), which is the victim frame of LRU scheme.

## Scheme3: Second-chance
Second-chance scheme is just an another variation of FIFO, it replaces the oldest page with ref bit 0 when page fault occurs.
If there is a free frame (also page fault), simply the new page occupies the frame.

My approach to implement Second-chance sceme is as follows:
I created a new element named *int ref* in the list *page_table_entry*.
Whenever a page found in a frame, increment ref bit of the page by one.
When page fault occured, find a page with the oldest time.
1: If ref bit of the found page is 0, then the frame is the victim.
2: Otherwise decrement ref bit by one and find the second oldest time.
Keep doing 1 and 2 until a victim frame is found.


