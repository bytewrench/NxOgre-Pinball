// OgreAudiere.cpp

#include "OgreAudiere.h"
#include <Ogre.h>
#include <audiere.h>

/*
namespace audiere
{
    namespace
    {
        // OgreResourceFile derived from audiere::File. Not for direct use
        class OgreResourceFile : public RefImplementation<File>
        {
        private:
            DataStreamPtr mData;

        public:
            OgreResourceFile(DataStreamPtr data)
                : mData(data)
            {
            }

            int read(void* buffer, int size)
            {
                return static_cast<int>(mData->read(buffer, static_cast<size_t>(size)));
            }

            bool seek(int position, SeekMode mode)
            {
                size_t pos = 0;
                switch (mode)
                {
                case CURRENT:
                    pos = mData->tell() + position;
                    break;
                case END:
                    pos = mData->size() + position;
                    break;
                default:
                    pos = position;
                    break;
                }
                mData->seek(pos);
                return true;
            }

            int tell()
            { 
                return static_cast<int>(mData->tell());
            }
        };
    }


    File* OpenOgreResourceFile(const Ogre::String& filename)
    {
        DataStreamPtr data = ResourceGroupManager::getSingleton().openResource(filename);
        return new OgreResourceFile(data);
    }
}
*/
