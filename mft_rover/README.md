#MFT_ROVER
Maker Fair Tokyo 2016に出展したHAKUTO×CHIRIMENのミニローバーです。
以下、このプログラムの使用方法。

1.PCとCHIRIMENを同一のネットワーク上に無線接続します。

2.シェル上でnode websocket.io.app.jsを実行し、サーバーを起動します。

$ node websocket.io.app.js
(node:85270) DeprecationWarning: process.EventEmitter is deprecated. Use require('events') instead.
 Server running localhost:8888 

3.index.htmlをブラウザで開きます。前進、後進、右回転、左回転の各動作が正しくできていれば正常です。

なお、2.で起動したサーバーは時間経過により落ちることがあるので、定期的に接続が切れていないか確認し、再起動してあげる必要があります。