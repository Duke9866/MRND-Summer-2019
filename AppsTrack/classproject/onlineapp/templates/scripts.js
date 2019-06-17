console.log('hi');

fetch('https://api.github.com/users/octocat')
    .then(result => {
        const jsonPr = result.json();
        jsonPr.then((data) => {
            console.log(data);
            console.log(data.name);
        });
    })

fetch('https://api.github.com/users/octocat')
    .then(result => {
        const jsonPr = result.json();
        jsonPr.then((data) => {
            console.log(data);
            console.log(data.name);
        });
    })
    .then((data) => {
        console.log(data);
    })


//const promiseArray = []
//for (let i = 0; i < 100; i++) {
//    const newPromise = new Promise((res, rej) => {
//        console.log(i);
//        res(i);
//    });
//    newPromise.then((number) => console.log('Done', number));
//    promiseArray.push(newPromise);
//}
//
//Promise.all(promiseArray).then(() => console.log('finish'));
//console.log('bye');







//const myFetch = new Promise((res, rej) => {
//    debugger;
//    //fake data fetch
//    const data = {
//        name: 'Someone',
//        email: 'someone@example.com'
//    };
//
//    setTimeout(() =>{
//        res(data);
//    }, 2000);
//});
//
//console.log('running')
//
//myFetch.then(data => console.log(data))
//    .catch(err => console.error('Error', err))
//
//console.log('bye bye');







//const sleep = new Promise((ayyindi, avvale) => {
//    setTimeout(() => {
//        console.log('delayed print');
//        ayyindi('done');
//    }, 2000);
//    avvale('Oops');
//});
//
//console.log(sleep);
//
//sleep
//    .then(data => console.log(data))
//    .catch(err => console.error('Error', err));