# katla-server

Source code for a server that receives data from Katla (see `DigesterControl`).
The data is stored in a SQLite database for retention and analysis. Further, the
server connects to the [Telegram Bot API](https://core.telegram.org/bots) to
allow access to the current status of the biodigester through Telegram. 

## Running

Running katla-server can be done using `npm` or any compatible flavor of it. We
use `pnpm` here.
```
pnpm i
pnpm build
pnpm start
```
