class CollisionManager: public Ogre::Singleton<CollisionManager> {
    public:b
        typedef boost::function<void(Body*, Body*)> CollisionCallback;
        enum CallbackType {BEGINCOLLISION, COLLIDING, ENDCOLLISION, ALL};
       
        CollisionManager();
        ~CollisionManager();

        static CollisionManager& getSingleton();
        static CollisionManager* getSingletonPtr();
        
        void addBody(Body* body);
        bool removeBody(Body* body);
        void removeAllBodies();

        void addCollisionCallback(int typeA,
                                  int typeB,
                                  CollisionCallback callback,
                                  CallbackType callbackType = BEGINCOLLISION);
        bool removeCollisionCallback(int typeA, int typeB, CallbackType callbackType = ALL);
        void removeAllCollisionCallbacks(CallbackType type = ALL);

        void checkCollisions();
    private:
        typedef boost::unordered_map<int, boost::unordered_map<int, CollisionCallback> > CollisionCallbackTable;
        typedef boost::unordered_map<Body*, std::set<Body*> > CollidingBodies;

        CollisionCallbackTable _beginCallbackTable;
        CollisionCallbackTable _inCallbackTable;
        CollisionCallbackTable _endCallbackTable;

        CollidingBodies _collidingBodies;
        std::list<Body*> _bodies;

        Sphere* _sphereA;
        Sphere* _sphereB;

        bool existsCallback(int typeA,
                            int typeB,
                            CallbackType calbackType,
                            CollisionCallback* collisionCallback);
                            
        bool wereColliding(Body* bodyA, Body* bodyB);
};
