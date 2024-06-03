//This is getting the sha key closes to the head of the repository
async function getLatestShaKey() {
    const { Octokit, App } = await import("octokit");

    const octokit = new Octokit();
    try {
        const response = await octokit.request('GET /repos/{owner}/{repo}/commits', {
            owner: 'Collegeville',
            repo: 'hpcg-ConnorBrynteson'
        });
        return [response.data[0].sha];
    } catch (error) {
        console.error(error);
        return [];
    }
}

//This is getting the sha key closes to the date entered or multiple if there are multiple commits on that day

async function getShaKeysByDate(date) {
    const { Octokit, App } = await import("octokit");

    const octokit = new Octokit();
    try {
        const response = await octokit.request('GET /repos/{owner}/{repo}/commits', {
            owner: 'Collegeville',
            repo: 'hpcg-ConnorBrynteson'
        });
        const shaKeys = response.data.filter(commit => commit.commit.author.date.includes(date)).map(commit => commit.sha);
        return shaKeys;
    } catch (error) {
        console.error(error);
        return [];
    }
}


// We want to use these sha keys to get the commit messages and summerize them in a markdown file
async function getCommitMessages(shaKeys) {
    const { Octokit, App } = await import("octokit");

    const octokit = new Octokit();
    try {
        const messages = await Promise.all(shaKeys.map(async sha => {
            const response = await octokit.request('GET /repos/{owner}/{repo}/git/commits/{commit_sha}', {
                owner: 'Collegeville',
                repo: 'hpcg-ConnorBrynteson',
                commit_sha: sha
            });
            return response.data.message;
        }));
        return messages;
    } catch (error) {
        console.error(error);
        return [];
    }
}

// Generate Markdown file with commit messages

async function generateMarkdown(messages) {
    const fs = await import("fs");

    const markdown = messages.reduce((acc, message) => {
        return acc + `* ${message}\n \n`;
    }, '');
    fs.writeFileSync('./docgen/commitMessages.md', markdown);
}

// Main function

async function main() {
    const currentShaKey = await getLatestShaKey();
    const shaKeys = await getShaKeysByDate('2020-02-17');
    const tempCommits = await getCommitMessages(shaKeys);
    await generateMarkdown(currentShaKey);
    await generateMarkdown(tempCommits);
}

main();