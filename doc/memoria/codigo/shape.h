class Shape {
    public:
        enum Type {SPHERE = 1, AABB = 2, PLANE = 3, OBB = 4};
        typedef boost::function<bool(Shape*, Shape*)> CollisionCheckFunction;

        Shape(const Ogre::String& name = "");
        virtual ~Shape();
        
        const Ogre::String& getName() const;

        virtual int getType() const = 0;

        virtual void applyTransform(Shape* localShape,
                                    const Ogre::Vector3& traslation = Ogre::Vector3::ZERO,
                                    const Ogre::Vector3& scale = Ogre::Vector3::UNIT_SCALE,
                                    const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY) = 0;

        virtual Shape* getTransformedCopy(const Ogre::Vector3& traslation = Ogre::Vector3::ZERO,
                                          const Ogre::Vector3& scale = Ogre::Vector3::UNIT_SCALE,
                                          const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY) = 0;

        static void configureCollisionDispatching();

        static void addCollisionTest(CollisionCheckFunction test, int typeA, int typeB);

        static bool getCollision(Shape* shapeA, Shape* shapeB);

    protected:
        Ogre::String _name;

    private:
        typedef boost::unordered_map<int, boost::unordered_map<int, CollisionCheckFunction> > CollisionDispatchTable;

        static CollisionDispatchTable  _collisionDispatcher;
        
        static bool getCollisionSphereSphere(Shape* shapeA, Shape* shapeB);
        static bool getCollisionAABBAABB(Shape* shapeA, Shape* shapeB);
        static bool getCollisionPlanePlane(Shape* shapeA, Shape* shapeB);
        static bool getCollisionOBBOBB(Shape* shapeA, Shape* shapeB);
        static bool getCollisionSphereAABB(Shape* shapeA, Shape* shapeB);
        static bool getCollisionSphereOBB(Shape* shapeA, Shape* shapeB);
        static bool getCollisionPlaneSphere(Shape* shapeA, Shape* shapeB);
        static bool getCollisionPlaneAABB(Shape* shapeA, Shape* shapeB);
        static bool getCollisionOBBPlane(Shape* shapeA, Shape* shapeB);
        static bool getCollisionOBBAABB(Shape* shapeA, Shape* shapeB);
};
