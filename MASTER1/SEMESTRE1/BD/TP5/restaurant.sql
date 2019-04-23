EXERCICE 3

1) db.restaurants.find().sort({name: 1}).pretty()
2) db.restaurants.find({cuisine:'Italian'}, {name:1, "address.zipcode":1, "address.coord":1}).pretty()
3) db.restaurants.find({"cuisine":'Italian', "telephoneNumber":{$exists:true},"address.zipcode": {$gt :'10075'} }).pretty()
4) db.restaurants.find({$or:[{"cuisine":'Italian'},{"address.zipcode":'10075'}]}).pretty()

EXERCICE 4

1) db.restaurants.update({"address.zipcode":'10016', "cuisine":'Other'},{$set:{"cuisine":'Cuisine to be determined'}}, {multi:true})

POUR VERIFIER : db.restaurants.find({"address.zipcode":'10016', "cuisine":'Cuisine to be determined'}).pretty()

2)db.restaurants.update({"restaurant_id": "41154403"},{$set:{"name":'Vella 2', "address.zipcode":'10075', "address.street":'2Avenue', "address.city":'1480'}}, {multi:true})

POUR VERIFIER : db.restaurants.find({"restaurant_id": "41154403"}).pretty()

EXERCICE 5

1)db.restaurants.aggregate([{$group: {_id:"$cuisine", myCount:{ $sum :1}}}])
2)db.restaurants.aggregate([{$match: {cuisine:"Italian"}},{$group: {_id:"$address.zipcode", myCount:{ $sum :1}}}])

EXERCICE 6

1) db.createCollection("comments")
2) db.comments.insert({"_id":"0","restaurant id":"40386672","name client":"Gui","comment":"Failli me noyer dans le bol de Frank. 0/5","type":"negative"})
db.comments.insert({"_id":"1","restaurant id":"40386672","name client":"xxbgdu38xx","comment":"Le bol de Frank est énoooooooorme Oo","type":"positive"})
db.comments.insert({"_id":"2","restaurant id":"40386672","name client":"Gui","comment":"Franchement cest pas si mal","type":"positive"})
db.comments.insert({"_id":"3","restaurant id":"40385587","name client":"Gui","comment":"OK","type":"positive"})
db.comments.insert({"_id":"4","restaurant id":"40385885","name client":"xxbgdu38","comment":"Jai eu du mal à trouver la bouffe, il sétait caché","type":"negative"})
db.comments.insert({"_id":"5","restaurant id":"40385606","name client":"Ronald Trompe","comment":"Alligato","type":"positive"})
