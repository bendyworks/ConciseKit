#define $shared(Klass) [Klass shared##Klass]

#ifdef ALLOW_ALLOC_INIT_FOR_SINGLETONS
  #define $singleton(Klass)\
  \
  static Klass *shared##Klass = nil;\
  \
  + (Klass *)shared##Klass {\
    @synchronized(self) {\
      if(shared##Klass == nil) {\
        shared##Klass = [[super allocWithZone:NULL] initSingleton];\
      }\
    }\
    return shared##Klass;\
  }\
  \
  - (id)init {\
    if((self = [super init])) {\
      IF_ARC(self =,) [self initSingleton];\
    }\
    return self;\
  }
#else
  #define $singleton(Klass)\
  \
  static Klass *shared##Klass = nil;\
  \
  + (Klass *)shared##Klass {\
    @synchronized(self) {\
      if(shared##Klass == nil) {\
        shared##Klass = [[super allocWithZone:NULL] init];\
      }\
    }\
    return shared##Klass;\
  }\
  \
  + (id)allocWithZone:(NSZone *)zone {\
    return IF_ARC([self shared##Klass], [[self shared##Klass] retain]);\
  }\
  \
  - (id)copyWithZone:(NSZone *)zone {\
    return self;\
  }\
  \
  IF_ARC(, \
    - (id)retain {\
      return self;\
    }\
    \
    - (NSUInteger)retainCount {\
      return NSUIntegerMax;\
    }\
    \
    - (void)release {}\
    \
    - (id)autorelease {\
      return self;\
    }\
  ) /*end IF_ARC */ \
  \
  + (BOOL)isShared##Klass##Present {\
    return shared##Klass != nil;\
  }\
  \
  - (id)init {\
    if(![[self class] isShared##Klass##Present]) {\
      if((self = [super init])) {\
        IF_ARC(self =,) [self initSingleton];\
      }\
    }\
    return self;\
  }
#endif
