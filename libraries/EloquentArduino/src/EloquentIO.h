//#include "io/InputStream/AnalogPinInputStream.h"
//#include "io/InputStream/ArrayInputStream.h"
//#include "io/InputStream/AssignableInputStream.h"
//#include "io/InputStream/AtLeast.h"
//#include "io/InputStream/Average.h"
//#include "io/InputStream/Baseline.h"
//#include "io/InputStream/Constrain.h"
//#include "io/InputStream/Derivative.h"
//#include "io/InputStream/EWMA.h"
//#include "io/InputStream/InputStream.h"
//#include "io/InputStream/Map.h"
//#include "io/InputStream/Round.h"
//#include "io/InputStream/Transformer.h"
#include "EloquentPin.h"

#if defined(SPIFFS_OK)
#include "io/Spiffs/Dir.h"
#include "io/Spiffs/File.h"
#endif