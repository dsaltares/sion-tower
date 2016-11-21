class StateManager: public Ogre::FrameListener,
                     public Ogre::WindowEventListener,
                     public OIS::KeyListener,
                     public OIS::MouseListener {
    public:
        StateManager(Game* game, OIS::InputManager* inputManager);
        ~StateManager();
        
        void start();
        void exitGame();
        
        // Control de la pila de estados
        void changeState(State* state);
        State* changeState(const Ogre::String& stateName);
        void popState();
        void pushState(State* state);
        void popAllStates();

        OIS::Mouse* getMouse();
        OIS::Keyboard* getKeyboard();
       
        // Eventos de entrada
        bool keyPressed(const OIS::KeyEvent &arg);
        bool keyReleased(const OIS::KeyEvent &arg);
        bool mouseMoved(const OIS::MouseEvent &arg);
        mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
        bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
        
        // Control del proceso de renderizado
        bool frameStarted(const Ogre::FrameEvent& event);
        bool frameEnded(const Ogre::FrameEvent& event);
        bool frameRenderingQueued(const Ogre::FrameEvent& event);
       
        // Eventos de ventana
        void windowClosed(Ogre::RenderWindow* window) {};
        void windowResized(Ogre::RenderWindow* window) {};
        bool windowClosing(Ogre::RenderWindow* window);
        
    private:
        // Juego
        Game* _game;

        // Objetos Ogre
        Ogre::Log* _log;
        Ogre::RenderWindow* _window;

        // Managers de entrada OIS
        OIS::InputManager* _inputManager;
        OIS::Keyboard* _keyboard;
        OIS::Mouse* _mouse;
        
        // Fichero de recursos
        Ogre::String _resourcesCfg;

        // Metodos auxiliares
        void configureOIS();
    
        // Vector de estados
        std::vector<State*> _states;
        
        // Iterador inverso
        std::vector<State*>::const_reverse_iterator rev_it;
        
        // Numero de estados (temporal)
        int _numStates;
        
        // Tipo de operacion (push o pop)
        enum tOperation {opPush, opPop};
        
        // Operacion de pop o push sobre un estado que se ejecutara
        // al comienzo de update
        struct StateOperation {
            tOperation _type;
            State* _state;
            
            StateOperation(tOperation type, State* state = 0): _type(type)
                                                               _state(state) {};
        };
        
        // Cola de operaciones por realizar
        std::queue<StateOperation> _pendingOperations;
        
        // Controlamos la salida
        bool _exit;
        
        // Realiza las operaciones pendientes
        void performOperations();
};
