#!/bin/bash

# Цвета для вывода
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${YELLOW}🚀 Запуск Todo-List с временной БД PostgreSQL...${NC}"

# 1. Запуск контейнера PostgreSQL
echo -e "${GREEN}📦 Запуск Docker-контейнера PostgreSQL...${NC}"
docker run -d \
  -v postgres_data:/var/lib/postgresql/data \
  -e POSTGRES_PASSWORD=123 \
  -e POSTGRES_DB=users \
  -e POSTGRES_USER=postgres \
  --name postgres_db \
  -p 5433:5432 \
  postgres:15

# 2. Ожидание готовности PostgreSQL
echo -e "${YELLOW}⏳ Ожидание готовности PostgreSQL...${NC}"
sleep 3

# Проверка, что контейнер запустился
if ! docker ps | grep -q postgres_db; then
    echo -e "${RED}❌ Ошибка: контейнер PostgreSQL не запустился${NC}"
    exit 1
fi

# 3. Создание базы данных tasks
echo -e "${GREEN}📋 Создание базы данных 'tasks'...${NC}"
docker exec postgres_db psql -U postgres -d users -c "CREATE DATABASE tasks;" 2>/dev/null
if [ $? -eq 0 ]; then
    echo -e "${GREEN}✅ База данных 'tasks' создана${NC}"
else
    echo -e "${YELLOW}⚠️ База данных 'tasks' уже существует${NC}"
fi

# 4. Создание таблицы tasks в БД tasks
echo -e "${GREEN}📋 Создание таблицы tasks...${NC}"
docker exec postgres_db psql -U postgres -d tasks -c "
CREATE TABLE IF NOT EXISTS tasks (
    task_id SERIAL PRIMARY KEY,
    task_status INTEGER DEFAULT 0,
    task_priority INTEGER DEFAULT 1,
    task_description TEXT,
    task_title VARCHAR(255) NOT NULL,
    task_deadline TIMESTAMP
);
"
if [ $? -eq 0 ]; then
    echo -e "${GREEN}✅ Таблица tasks создана${NC}"
else
    echo -e "${RED}❌ Ошибка при создании таблицы${NC}"
fi

# 5. Запуск приложения
echo -e "${GREEN}🎯 Запуск Todo-List...${NC}"
cd ./build
./todo
cd ..

# 6. Остановка и удаление контейнера
echo -e "${YELLOW}🛑 Остановка и удаление контейнера PostgreSQL...${NC}"
docker stop postgres_db
docker rm postgres_db

echo -e "${GREEN}✅ Готово!${NC}"
