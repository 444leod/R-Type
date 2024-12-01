FROM node:22

LABEL org.opencontainers.image.source https://github.com/EpitechPromo2027/B-CPP-500-LIL-5-2-rtype-leo.dumont

WORKDIR /app

COPY . .

RUN npm install

CMD ["npm", "run", "serve", "--", "--port", "3000"]
