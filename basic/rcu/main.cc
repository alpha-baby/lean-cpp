#include <iostream>
#include <thread>
#include <chrono>
#include <deque>

using Params = std::shared_ptr<int>; 

thread_local Params tls_params;

using Queue = std::deque<Params>;

void worker(const char* name, Params init_params, Queue* queue){

  tls_params = init_params;

  while(true){
    //read params without lock
    std::cout << "worker " << name << " " << *tls_params.get() << std::endl;

    //message loop
    while(!queue->empty()) {
      //update params
      tls_params = queue->front();
      queue->pop_front();

      //signal to exit
      if (tls_params == nullptr){
        return;
      }
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}
// build g++ -std=c++11 main.cc
int main(){

  //create worker and pass initia params
  auto init_params = std::make_shared<int>(1);
  auto q1 = Queue();
  auto q2 = Queue();
  std::thread w1(worker, "w1", init_params, &q1);
  std::thread w2(worker, "w2", init_params, &q2);

  for(int i =0; i < 10; i++){

    //push new params to worker
    auto new_params = std::make_shared<int>(i);
    q1.push_back(new_params);
    q2.push_back(new_params);
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  //sigal worker to exit
  q1.push_back(nullptr);
  q2.push_back(nullptr);

  w1.join();
  w2.join();

  return 0;
}