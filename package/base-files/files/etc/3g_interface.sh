uci set network.ppp0='interface'
uci set network.ppp0.proto='3g'
uci set network.ppp0.service='umts'
#uci set network.3g.username='root'
#uci set network.3g.password='123456'
uci set network.ppp0.device='/dev/ttyUSB3'
uci set network.ppp0.apn='internet'
uci commit

