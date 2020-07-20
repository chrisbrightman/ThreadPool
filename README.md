# Basic Thread Pool

This a very basic thread pool that I impelmented because I was curious about 
learing more about how they work and so that I could learn C++.

### Usage

Standard usage would be something similar to

```
ThreadPool<int> pool;
pool.addWork([] () { return add( 1, 1); });
```

- The add work funcion must take a lamnda or a bound function using std::bind
- When the pool variable is destroyed it will wait for the all of the work to be completed
- The maximum number of thread worker threads that the pool will create defaults to 15 * hardware_concurrency 
- This can be changed by using the other constructor and simply passing the maximum as a paramter
- The Thread Pool does grow dynamically and has a managment thread to do so 


