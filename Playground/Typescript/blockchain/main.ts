const { createHmac } = await import('node:crypto');

const secret = '0ad7889';

type Block<Payload> = {
  id: number;
  previous?: Block<Payload>;
  next?: Block<Payload>;
  nounce: number;
  payload: Payload;
  hash: string;
}

const create_genesis = <T>(): Block<T> => {
  return {
    id: 0,
    previous: undefined,
    next: undefined,
    nounce: 0,
    hash: `0`
  } as Block<T>;
}

const insert_block = <T>(chain: Block<T>, payload: T): Block<T> => {
  let cruiser: Block<T> = chain;

  while (cruiser.next)
    cruiser = cruiser.next;

  cruiser.next = mine(cruiser, payload);

  return chain;
}

function hash_SHA256_sync(data: string): string {
  return createHmac('sha256', secret)
    .update(data)
    .digest('hex');
}

const hash_block = <T>(block: Block<T>): string => {
  return hash_SHA256_sync(JSON.stringify({ ...block, hash: "", previous: undefined, next: undefined }));
}

const validate_block = <T>(block: Block<T>): boolean => {
  return 0 == block.id || hash_block(block).startsWith(secret);
}

const validate_chain = <T>(chain: Block<T>): boolean => {
  if(0 == chain.id) console.log(`\n========= Validating chain ==========`)
  console.log(`Block ${chain.id}: ${validate_block(chain)}`);
  return validate_block(chain) && (chain.next != undefined ? validate_chain(chain.next) : true);
}

const mine = <T>(last_block: Block<T>, payload: T): Block<T> => {
  const new_block: Block<T> = {
    id: last_block.id + 1,
    previous: last_block,
    next: undefined,
    nounce: 0,
    hash: "",
    payload
  };

  while (!validate_block(new_block)) {
    console.log(`> Mining block ${new_block.id}. Trying nonce: ${new_block.nounce} - Order => ${Math.floor(Math.log10(new_block.nounce))}\n>> ${hash_block(new_block)}`);
    new_block.nounce++;
  }

  new_block.hash = hash_block(new_block);

  return new_block;
}

const print_chain = <T>(chain: Block<T>): void => {
  let iterator: Block<T> | undefined = chain;
  do {
    console.log({
      id: iterator.id,
      nounce: iterator.nounce,
      hash: iterator.hash,
      payload: iterator.payload,
      valid: validate_block(iterator)
    });
    iterator = iterator.next;
  } while (iterator);
}


// EXEC
type Payload = {
  id: number,
  name: string,
  category: string,
  price: number,
  stock: number,
  description: string,
  isAvailable: boolean
};

const bchain = create_genesis<Payload>();

[
    {
        "id": 1,
        "name": "Product 1",
        "category": "Category 1",
        "price": 23.45,
        "stock": 10,
        "description": "This is a description for product 1.",
        "isAvailable": true
    },
    {
        "id": 2,
        "name": "Product 2",
        "category": "Category 1",
        "price": 12.99,
        "stock": 5,
        "description": "This is a description for product 2.",
        "isAvailable": false
    },
    {
        "id": 3,
        "name": "Product 3",
        "category": "Category 1",
        "price": 45.00,
        "stock": 20,
        "description": "This is a description for product 3.",
        "isAvailable": true
    },
    {
        "id": 4,
        "name": "Product 4",
        "category": "Category 2",
        "price": 99.99,
        "stock": 0,
        "description": "This is a description for product 4.",
        "isAvailable": false
    },
    {
        "id": 5,
        "name": "Product 5",
        "category": "Category 2",
        "price": 15.75,
        "stock": 30,
        "description": "This is a description for product 5.",
        "isAvailable": true
    },
    {
        "id": 6,
        "name": "Product 6",
        "category": "Category 2",
        "price": 34.50,
        "stock": 7,
        "description": "This is a description for product 6.",
        "isAvailable": true
    },
    {
        "id": 7,
        "name": "Product 7",
        "category": "Category 3",
        "price": 8.99,
        "stock": 15,
        "description": "This is a description for product 7.",
        "isAvailable": false
    },
    {
        "id": 8,
        "name": "Product 8",
        "category": "Category 3",
        "price": 50.00,
        "stock": 25,
        "description": "This is a description for product 8.",
        "isAvailable": true
    },
    {
        "id": 9,
        "name": "Product 9",
        "category": "Category 3",
        "price": 19.95,
        "stock": 13,
        "description": "This is a description for product 9.",
        "isAvailable": true
    },
    {
        "id": 10,
        "name": "Product 10",
        "category": "Category 4",
        "price": 12.50,
        "stock": 20,
        "description": "This is a description for product 10.",
        "isAvailable": false
    },
    {
        "id": 11,
        "name": "Product 11",
        "category": "Category 4",
        "price": 44.99,
        "stock": 5,
        "description": "This is a description for product 11.",
        "isAvailable": true
    },
    {
        "id": 12,
        "name": "Product 12",
        "category": "Category 4",
        "price": 60.00,
        "stock": 10,
        "description": "This is a description for product 12.",
        "isAvailable": true
    },
    {
        "id": 13,
        "name": "Product 13",
        "category": "Category 1",
        "price": 29.99,
        "stock": 0,
        "description": "This is a description for product 13.",
        "isAvailable": false
    },
    {
        "id": 14,
        "name": "Product 14",
        "category": "Category 1",
        "price": 39.99,
        "stock": 12,
        "description": "This is a description for product 14.",
        "isAvailable": true
    },
    {
        "id": 15,
        "name": "Product 15",
        "category": "Category 2",
        "price": 22.50,
        "stock": 18,
        "description": "This is a description for product 15.",
        "isAvailable": true
    },
    {
        "id": 16,
        "name": "Product 16",
        "category": "Category 3",
        "price": 11.99,
        "stock": 8,
        "description": "This is a description for product 16.",
        "isAvailable": true
    },
    {
        "id": 17,
        "name": "Product 17",
        "category": "Category 3",
        "price": 30.00,
        "stock": 9,
        "description": "This is a description for product 17.",
        "isAvailable": false
    },
    {
        "id": 18,
        "name": "Product 18",
        "category": "Category 4",
        "price": 99.99,
        "stock": 3,
        "description": "This is a description for product 18.",
        "isAvailable": true
    },
    {
        "id": 19,
        "name": "Product 19",
        "category": "Category 4",
        "price": 37.49,
        "stock": 14,
        "description": "This is a description for product 19.",
        "isAvailable": true
    },
    {
        "id": 20,
        "name": "Product 20",
        "category": "Category 4",
        "price": 18.95,
        "stock": 2,
        "description": "This is a description for product 20.",
        "isAvailable": false
    }
].forEach((el: Payload) => insert_block(bchain, el));




console.log(validate_chain(bchain));

if (
  bchain.next &&
  bchain.next.next &&
  bchain.next.next.next &&
  bchain.next.next.next.next &&
  bchain.next.next.next.next.next &&
  bchain.next.next.next.next.next.next &&
  bchain.next.next.next.next.next.next.next
) bchain.next.next.next.next.next.next.next.nounce = 0;

console.log(validate_chain(bchain));

print_chain(bchain);




//const chain = create_genesis<{ name: string }>();
//
//insert_block(chain, { name: `Vinicius` });
//insert_block(chain, { name: `Mariana` });
//insert_block(chain, { name: `Josiane` });
//insert_block(chain, { name: `Antoniel` });
//insert_block(chain, { name: `Paulo` });
//insert_block(chain, { name: `Flor` });
//insert_block(chain, { name: `Mel` });
//insert_block(chain, { name: `Jessie` });
//insert_block(chain, { name: `Batman` });
//insert_block(chain, { name: `Tomate` });
//insert_block(chain, { name: `Poiseh` });
//
//print_chain(chain);
//
//console.log(validate_chain(chain));
//
//if (chain.next && chain.next.next && chain.next.next.next) chain.next.next.next.nounce = 0;
//
//console.log(validate_chain(chain));
