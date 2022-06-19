// 下列 ifdef 區塊是建立巨集的標準方式，讓從 DLL 匯出的過程更簡單。
// 此 DLL 中的所有檔案均使用命令列中定義的 XSOCKET_EXPORTS 來編譯。
// 此符號不應在其他使用此 DLL 的任何專案中定義
// 這樣一來，原始程式檔中包含此檔案的其他任何專案，
// 都會將 XSOCKET_API 函式視為從 DLL 匯入，
// 而此 DLL 會將使用此巨集定義的符號視為匯出。
#ifdef XSOCKET_EXPORTS
#define XSOCKET_API __declspec(dllexport)
#else
#define XSOCKET_API __declspec(dllimport)
#endif

// 這個類別是從 dll 匯出的
class XSOCKET_API Cxsocket {
public:
	Cxsocket(void);
	// TODO: 在此新增您的方法。
};

extern XSOCKET_API int nxsocket;

XSOCKET_API int fnxsocket(void);
