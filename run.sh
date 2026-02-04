#!/bin/bash
docker run -d -v postgres_data:/var/lib/postgresql/data -e POSTGRES_PASSWORD=123 -e POSTGRES_DB=users -e POSTGRES_USER=postgres \
--name postgres_db -p 5433:5432 postgres:15
sleep 3
cd ./build
./todo
cd ..
docker stop postgres_db
docker rm postgres_db
