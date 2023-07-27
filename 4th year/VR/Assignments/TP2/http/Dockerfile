FROM node:alpine

WORKDIR /http

COPY package.json ./

RUN npm install 

COPY . .

CMD ["npm", "start"]

# FROM node:latest

# WORKDIR /auth

# COPY package.json .

# RUN npm install

# COPY . .

# EXPOSE 5000

# VOLUME [ "/auth/node_modules" ]

# CMD ["npm","start"]