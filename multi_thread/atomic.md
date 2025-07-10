# 1. Cache Conherence（缓存一致性模型）

## 1.1. CPU存储结构

CPU能直接访问的存储结构，由快到慢：
- 寄存器(Register)
- cache
  - L1 Cache (不同CPU核之间相互独立)
  - L2 Cache (通常是独立的)
  - L3 Cache (通常是共享的)
- 内存

## 1.2. 缓存一致性协议MESI

由于写内存时采用写通+不按写分配的策略，由于L1和L2缓存对其他CPU是不可见的，当一个cpu更新自己的缓存数据时，其他CPU没同步，因此需要写广播，CPU需要将每个修改动作都通过总线进行广播，并实时监听总线上的消息。而这种做法在多核系统会导致性能急剧下降，因此发明了MESI进行优化。

MESI是一个状态机，其状态分为：
  - **M**odified (已修改)
    该缓存行刚刚被修改过，且保证不会出现在其他CPU核心的缓存中。此时该CPU核心为对应数据的唯一持有者。
  - **E**xclusive (独占)
    该缓存行的数据目前由该CPU核心独占，但因为没有被修改过，因此与内存数据也一致。
  - **S**hared (共享)
    该缓存行的内容与至少一个其他CPU所共享，因此CPU核心修改此缓存行时需要与其他CPU核心协商。
  - **I**nvalid (无效)
    该缓存行为无效行。

当一段数据在多个CPU核心中共享时，CPU核心在更新数据时需要通过总线发送Invalidate广播消息，等到其他CPU核心将缓存行置为Invalid状态后才能修改缓存数据。

# 2. Memory Consistency（内存一致性）

## 2.1. 内存乱序原因

### 2.1.1. 编译器乱序
指令重排

### 2.1.2. 处理器乱序
Store Buffer
在CPU和缓存之间存在一个Store Buffer，写操作不会立即向其他CPU核心发送Invalidate消息，而是先将写操作放入到Store Buffer中，然后即可返回执行其他的操作，具体的写入和同步操作由Store Buffer来执行。如果在Store Buffer中的数据写入缓存前就需要读取它，还需要从Store Buffer读取数据。

Invalidate Queue
由于Store Buffer容量有限，如果Store Buffer发送Invalidate消息后，接受消息的CPU核心无法及时响应，则Store Buffer很快就会被写满，因此CPU还引入了Invalidata Queue。其他的CPU核心会将消息放入本核心的Invalidata Queue后立即确认Invalidate消息，而无需等待相关的缓存无效。CPU核心在发送Invalidata消息前必须先处理完还未处理完的Invalidate消息。

Store Buffer引入了写入延后，Invalidate Queue引入了读取提前 (导致数据延迟更新，从而使得CPU读取到旧数据，看起来像是读取提前了)，由于这两者的存在而导致多核心数据同步时出现乱序的问题。

## 2.2. 内存屏障

### 2.2.1. Load-Load 屏障
读取时先处理完 Invalidate Queue 

### 2.2.2. Store-Store 屏障
写入后立刻处理 Store Buffer 

### 2.2.3. Load-Store 屏障

### 2.2.4. Store-Load 屏障
同时处理 Store Buffer 和 Invalidata Queue

# 3. C++ 的 Memory Order