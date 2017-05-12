#### class dlib_cc::lts_map_unit::InfoJsonParser : public cocos2d::Ref
# InfoJsonParser
`LTS-Map`のMap,Layer,Bundler,Atlas,Terrainのそれぞれの設定ファイル(`.json`)をパースして、各infoオブジェクトを作成する。  
各infoオブジェクトの実体は`InfoJsonParser`オブジェクトが持ち、データを参照するときはアクセッサメソッドでポインタを受け取り、それを利用する。  
使用できるデータ形式、キー値は以下を参照。。。  
###### < データ形式 >  
| Data Format | Example |
|:-----------:|:-------|
|map|"key" : { "key1" : value1, "key2" : value2, "key3" : value3, ...}|
|array|"key" : [value1, value2, value3, ...]|
|vec2|"key" : [x, y]|
|size|"key" : [width, height]|
|string|"key" : "somethig string"|
|numerics|"key" : something numerics|
|boolean|"key" : true or false|

###### < キー値 >  
| Key | Data Format | Summary |
|:---:|:-----------:|:-------|
|map_info|map|Mapの設定|  
|layer_info|map|Layerの設定|  
|bundler_info|map|Bundlerの設定|  
|atlas_info|map|Atlasの情報|  
|terrain_info|map|Terrainの情報|  
|file_index|string in array|続けてパースする`.json`ファイル名の一覧|  
|architecture|string in array|Map(Bundler)を構成するBundler(Layer)|  
|bundler_name|string|Bundler名|  
|layer_name|string|Layer名|  
|atlas_name|string|Atlas名|  
|terrain_name|string|Terrain名|  
|atlas_src_name|string|タイルのテクスチャアトラスのファイル名|  
|terrain_src_name|string|地形データのファイル名|  
|is_visible|boolean|Layerが描画されるかどうか|  
|is_editable|boolean|地形データが改変可能かどうか|  
|num_tile_type|numerics|タイルの種類の数|  
|tile_type_no_tile|numerics|タイルが存在しないことを示すフラグ値|  
|texture_size|size|タイル１枚分のテクスチャのサイズ|  
|map_size|size|マップ全体のサイズ（縦のタイル数　x　横のタイル数）|  
|block_size|size|ブロックのサイズ（縦のタイル数　x　横のタイル数）|  
|texture_positions|vec2 in array|テクスチャアトラス内での、各タイルの座標|  
|location_pin_map|map(value = vec2)|保存したマップ上の座標とその場所の名称|  

パースするファイルが複数ある場合、以下のようなインデックスファイルを用意し、これをパースすることで、他のA、B、Cのファイルも自動的にパースすることができる。
```
// index.json

{
	"file_index" : [ "fileA.json, fileB.json, fileC.json"]
}
```

#### 名前空間
```c++
namespace dlib_cc
{
	namespace lts_map_unit
	{
		class InfoJsonParser;
	}
}

```

#### private メンバ変数
```c++
MapInfo map_info_;
std::unordered_map<std::string, LayerInfo> layer_info_map_;
std::unordered_map<std::string, BundlerInfo> bundler_info_map_;
std::unordered_map<std::string, AtlasInfo> atlas_info_map_;
std::unordered_map<std::string, TerrainInfo> terrain_info_map_;
```

`MapInfo map_info_`
>Map全体の設定

`std::unordered_map<std::string, LayerInfo> layer_info_map_`
>各Layerの個別の設定

`std::unordered_map<std::string, BundlerInfo> bundler_info_map_`
>各Bunderの個別の設定

`std::unordered_map<std::string, AtlasInfo> atlas_info_map_`
>各texture atlasの個別の情報

`std::unordered_map<std::string, TerrainInfo> terrain_info_map_`
>各Terrainの個別の情報

#### public メンバ関数
```c++
static InfoJsonParser* Create();
bool ParseJson(std::string json);
inline const MapInfo* map_info() const;
inline const LayerInfo* layer_info(std::string& name) const;
inline const BundlerInfo* bundler_info(std::string& name) const;
inline const AtlasInfo* atlas_info(std::string& name) const;
inline const TerrainInfo* terrain_info(std::string& name) const;
```

`static InfoJsonParser* Create()`
>cocosのcreate()メソッド

`bool ParseJson(std::string json)`
>**@param json :** パースする`.json`ファイル名(パスではない)  

>渡された`.json`ファイルをパース、infoオブジェクトを作成しそれをメンバのコンテナ（または変数)に格納する。  

`inline const MapInfo* map_info() const`
>MapInfoオブジェクトのポインタを取得

`inline const LayerInfo* layer_info(std::string& name) const`
>**@param name :** 特定のLayer名  

>Layer名からLayerInfoオブジェクトのポインタを取得

`inline const BundlerInfo* bundler_info(std::string& name) const`
>**@param name :** 特定のBundler名  

>Bundler名からLayerInfoオブジェクトのポインタを取得

`inline const AtlasInfo* atlas_info(std::string& name) const`
>**@param name :** 特定のAtlas名  

>Atlas名からAtlasInfoオブジェクトのポインタを取得

`inline const TerrainInfo* terrain_info(std::string& name) const`
>**@param name :** 特定のTerrain名  

>Terrain名からTerrainInfoオブジェクトのポインタを取得

#### protected メンバ関数
```c++
bool Init();
```

`bool Init()`
>初期化メソッド。Create()内で呼ばれる。初期化が成功すれば`ture`、そうでなければ`falise`をかえす。

#### private メンバ関数
```c++
MapInfo MakeMapInfo(picojson::value& obj);
LayerInfo MakeLayerInfo(picojson::value& obj);
BundlerInfo MakeBundlerInfo(picojson::value& obj);
AtlasInfo MakeAtlasInfo(picojson::value& obj);
TerrainInfo MakeTerrainInfo(picojson::value& obj);
template <typename T> dlib::size<T> MakeDLibSize(picojson::value& value);
template <typename T> dlib::vec2<T> MakeDLibVec2(picojson::value& value);
```

`MapInfo MakeMapInfo(picojson::value& obj)`
>**@param obj :** パースされたMapのデータ群  

>MapInfoのデータをもつpicojsonのオブジェクトから、MapInfoオブジェクトを作成して、返却

`LayerInfo MakeLayerInfo(picojson::value& obj)`
>**@param obj :** パースされたLayerのデータ群  

>LayerInfoのデータをもつpicojsonのオブジェクトから、LayerInfoオブジェクトを作成して、返却

`BundlerInfo MakeBundlerInfo(picojson::value& obj)`
>**@param obj :** パースされたBundlerのデータ群  

>BundlerInfoのデータをもつpicojsonのオブジェクトから、BundlerInfoオブジェクトを作成して、返却

`AtlasInfo MakeAtlasInfo(picojson::value& obj)`
>**@param obj :** パースされたAtlasのデータ群  

>AtlasInfoのデータをもつpicojsonのオブジェクトから、AtlasInfoオブジェクトを作成して、返却

`TerrainInfo MakeTerrainInfo(picojson::value& obj)`
>**@param obj :** パースされたTerrainのデータ群  

>TerrainInfoのデータをもつpicojsonのオブジェクトから、TerrainInfoオブジェクトを作成して、返却

`template <typename T> dlib::size<T> MakeDLibSize(picojson::value& value)`
>**@param value :** パースされた、データ形式がsizeのデータ  

>dlib::size<T>クラスのオブジェクトを`picojson::value`から作成して、返却

`template <typename T> dlib::vec2<T> MakeDLibVec2(picojson::value& value)`
>**@param value :** パースされた、データ形式がvec2のデータ  

>dlib::vec2<T>クラスのオブジェクトを`picojson::value`から作成して、返却
