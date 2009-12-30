static int sphereCount = 0;

class PinBall
    {
        public:

            Body * myBody;
            Body * tmpBod;
            SphereShape * _pinBall;
            int ballState;
			Real _maxSpeed;
			PinBall ( NxVec3 pos, float size, float mass,Real maxSpeed, bool boxMode = true );
			void update (Real slope);
			void capSpeed(int max);
            
    };

