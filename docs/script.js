document.querySelectorAll('a[href^="#"]').forEach(anchor => {
  anchor.addEventListener('click', function (e) {
    e.preventDefault();
    const target = document.querySelector(this.getAttribute('href'));

    if (target) {
      const navHeight = document.querySelector('.navbar').offsetHeight;
      const targetPosition = target.offsetTop - navHeight;

      window.scrollTo({
        top: targetPosition,
        behavior: 'smooth'
      });

      updateActiveNavLink(this.getAttribute('href'));
    }
  });
});

function updateActiveNavLink(hash) {
  document.querySelectorAll('.nav-link').forEach(link => {
    link.classList.remove('active');
    if (link.getAttribute('href') === hash) {
      link.classList.add('active');
    }
  });
}

const sections = document.querySelectorAll('section[id]');
const navLinks = document.querySelectorAll('.nav-link');

const observerOptions = {
  root: null,
  rootMargin: '-50% 0px -50% 0px',
  threshold: 0
};

const sectionObserver = new IntersectionObserver((entries) => {
  entries.forEach(entry => {
    if (entry.isIntersecting) {
      const id = entry.target.getAttribute('id');
      navLinks.forEach(link => {
        link.classList.remove('active');
        if (link.getAttribute('href') === `#${id}`) {
          link.classList.add('active');
        }
      });
    }
  });
}, observerOptions);

sections.forEach(section => sectionObserver.observe(section));

const tabButtons = document.querySelectorAll('.tab-btn');

tabButtons.forEach(button => {
  button.addEventListener('click', () => {
    const tab = button.getAttribute('data-tab');

    tabButtons.forEach(btn => btn.classList.remove('active'));

    const tabCodeBlocks = document.querySelectorAll('.code-block[data-content]');
    tabCodeBlocks.forEach(block => block.classList.remove('active'));

    button.classList.add('active');
    const targetBlock = document.querySelector(`.code-block[data-content="${tab}"]`);
    if (targetBlock) {
      targetBlock.classList.add('active');
    }
  });
});

const copyButtons = document.querySelectorAll('.copy-btn');

copyButtons.forEach(button => {
  button.addEventListener('click', async () => {
    const copyType = button.getAttribute('data-copy');
    let textToCopy = '';

    switch (copyType) {
      case 'macos':
        textToCopy = `brew install ncurses
export LC_ALL=en_US.UTF-8
export LANG=en_US.UTF-8`;
        break;
      case 'linux':
        textToCopy = `sudo apt-get install libncurses5-dev libncurses-dev
sudo apt-get install locales
sudo locale-gen en_US.UTF-8`;
        break;
      case 'build':
        textToCopy = `git clone https://github.com/notCienki/tron-cli-game
cd tron-cli-game
make`;
        break;
      case 'run':
        textToCopy = `./tron`;
        break;
    }

    try {
      await navigator.clipboard.writeText(textToCopy);

      const originalHTML = button.innerHTML;
      button.innerHTML = `
                <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                    <polyline points="20 6 9 17 4 12"></polyline>
                </svg>
                Copied!
            `;
      button.style.background = 'rgba(6, 255, 165, 0.2)';
      button.style.borderColor = 'var(--success)';
      button.style.color = 'var(--success)';

      setTimeout(() => {
        button.innerHTML = originalHTML;
        button.style.background = '';
        button.style.borderColor = '';
        button.style.color = '';
      }, 2000);
    } catch (err) {
      console.error('Failed to copy text: ', err);
      button.innerHTML = 'Failed';
      setTimeout(() => {
        button.innerHTML = originalHTML;
      }, 2000);
    }
  });
});

const animateOnScroll = () => {
  const elements = document.querySelectorAll('.feature-card, .doc-card, .step');

  const observer = new IntersectionObserver((entries) => {
    entries.forEach(entry => {
      if (entry.isIntersecting) {
        entry.target.style.opacity = '1';
        entry.target.style.transform = 'translateY(0)';
      }
    });
  }, {
    threshold: 0.1,
    rootMargin: '0px 0px -100px 0px'
  });

  elements.forEach(element => {
    element.style.opacity = '0';
    element.style.transform = 'translateY(30px)';
    element.style.transition = 'opacity 0.6s ease, transform 0.6s ease';
    observer.observe(element);
  });
};

animateOnScroll();

const terminalText = document.querySelector('.terminal-text');
if (terminalText) {
  const originalText = terminalText.innerHTML;
  let index = 0;
}

let ticking = false;

function updateParallax() {
  const scrolled = window.pageYOffset;
  const gridBg = document.querySelector('.grid-bg');

  if (gridBg) {
    const yPos = scrolled * 0.5;
    gridBg.style.transform = `perspective(500px) rotateX(60deg) translateY(${yPos}px)`;
  }

  ticking = false;
}

window.addEventListener('scroll', () => {
  if (!ticking) {
    window.requestAnimationFrame(updateParallax);
    ticking = true;
  }
});

const navbar = document.querySelector('.navbar');
let lastScroll = 0;

window.addEventListener('scroll', () => {
  const currentScroll = window.pageYOffset;

  if (currentScroll > 100) {
    navbar.style.background = 'rgba(10, 14, 39, 0.95)';
    navbar.style.boxShadow = '0 4px 20px rgba(0, 0, 0, 0.3)';
  } else {
    navbar.style.background = 'rgba(10, 14, 39, 0.8)';
    navbar.style.boxShadow = 'none';
  }

  lastScroll = currentScroll;
});

document.querySelectorAll('.btn-primary').forEach(button => {
  button.addEventListener('mousemove', (e) => {
    const rect = button.getBoundingClientRect();
    const x = e.clientX - rect.left;
    const y = e.clientY - rect.top;

    button.style.setProperty('--mouse-x', `${x}px`);
    button.style.setProperty('--mouse-y', `${y}px`);
  });
});

const createMobileMenu = () => {
  const navLinks = document.querySelector('.nav-links');
  const mobileToggle = document.createElement('button');
  mobileToggle.className = 'mobile-toggle';
  mobileToggle.innerHTML = '☰';
  mobileToggle.style.display = 'none';

  if (window.innerWidth <= 768) {
    mobileToggle.style.display = 'block';
    document.querySelector('.nav-container').prepend(mobileToggle);

    mobileToggle.addEventListener('click', () => {
      navLinks.classList.toggle('mobile-active');
    });
  }
};

window.addEventListener('resize', createMobileMenu);
createMobileMenu();

document.addEventListener('keydown', (e) => {
  if (e.key === 'g' && !e.ctrlKey && !e.metaKey) {
    setTimeout(() => {
      document.addEventListener('keydown', function openGitHub(event) {
        if (event.key === 'h') {
          window.open('https://github.com/notCienki/tron-cli-game', '_blank');
        }
        document.removeEventListener('keydown', openGitHub);
      });
    }, 500);
  }
});

let konamiCode = [];
const konamiPattern = ['ArrowUp', 'ArrowUp', 'ArrowDown', 'ArrowDown', 'ArrowLeft', 'ArrowRight', 'ArrowLeft', 'ArrowRight', 'b', 'a'];

document.addEventListener('keydown', (e) => {
  konamiCode.push(e.key);
  konamiCode = konamiCode.slice(-10);

  if (konamiCode.join('') === konamiPattern.join('')) {
    document.body.style.animation = 'rainbow 2s linear infinite';
    setTimeout(() => {
      document.body.style.animation = '';
    }, 3000);

    const message = document.createElement('div');
    message.innerHTML = '🎮 TRON MODE ACTIVATED! 🎮';
    message.style.cssText = `
            position: fixed;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            background: var(--primary);
            color: var(--bg-dark);
            padding: 2rem 3rem;
            border-radius: 12px;
            font-size: 2rem;
            font-weight: 800;
            z-index: 10000;
            animation: fadeInUp 0.6s ease;
        `;
    document.body.appendChild(message);

    setTimeout(() => {
      message.remove();
    }, 3000);
  }
});

const style = document.createElement('style');
style.textContent = `
    @keyframes rainbow {
        0% { filter: hue-rotate(0deg); }
        100% { filter: hue-rotate(360deg); }
    }
`;
document.head.appendChild(style);

console.log('%c🎮 TRON CLI GAME 🎮', 'color: #00d9ff; font-size: 24px; font-weight: bold;');
console.log('%cBuilt with ❤️ by notCienki', 'color: #8c92ac; font-size: 14px;');
console.log('%cCheck out the code: https://github.com/notCienki/tron-cli-game', 'color: #00d9ff; font-size: 12px;');

function debounce(func, wait) {
  let timeout;
  return function executedFunction(...args) {
    const later = () => {
      clearTimeout(timeout);
      func(...args);
    };
    clearTimeout(timeout);
    timeout = setTimeout(later, wait);
  };
}

window.addEventListener('load', () => {
  document.body.style.opacity = '0';
  document.body.style.transition = 'opacity 0.5s ease';

  setTimeout(() => {
    document.body.style.opacity = '1';
  }, 100);
});