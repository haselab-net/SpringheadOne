LocalDynamicsの使い方　2005.3.1

既存のxファイルに、
オブジェクトDummySphereを追加してください
　・ソリッドの名前：soDummySphere
　・フレームの名前：frDummySphere
として、
大きさは任意に設定してかまいませんが、
ポインタよりは大きくしてください
（はじめはDummySphereを表示して大きさを設定し、
　大きさが定まったら透明にするといいと思います

ContactEngineにDummySphereを追加し、
DummySphereとPointerに対して、
ほかのすべての物体をContactInactiveに設定します

設定は以上です
